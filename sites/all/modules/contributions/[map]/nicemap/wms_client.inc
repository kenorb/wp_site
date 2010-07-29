<?php
// $Id: wms_client.inc,v 1.1.2.11 2009/02/11 18:49:15 tmcw Exp $

/**
 * @file wms_client: a system-independent 
 * library for WMS clients
 */

/**
 * A map object.
 */
class nicemap_map {

  public $url;
  public $exceptions     = array();
  public $filetypes      = array();
  public $identification = array();
  public $operations     = array();
  public $layers = array();
  public $styles = array();
  public $bounds = array();
  public $bgcolor;
  public $projection;

  /**
    * Construct a new nicemap_map object
    *
    * @param $url URL of the WMS server, without a request or service
    * parameter
    * @param $spec Optional array of all the GetCapabilities information
    * that nicemap_needs to work. If this isn't provided, load_spec()
    * is automatically called and that information is loaded
    * @return new nicemap_map object
   */
  function __construct($url, $spec = '') {
    $this->url = $url;

    // Load a provided spec if one is given
    if(is_array($spec)) {
      foreach($spec as $k=>$v) {
        $this->$k = $v;
      }
    }
    else {
      $this->load_spec($url);
    }


  }

  /**
   * Dump all of the data returned by GetCapabilities
   * to an array. This function will most likely be used in order to
   * cache server capabilities, like
   \Example
   \code
   * if($spec = cache($url))
   *   $map = new nicemap_map($url, $spec);
   * else
   *   $map = new nicemap_map($url);
   *   set_cache($url, $map->dump());
   \endcode
   * @param none
   * @return array of GetCapabilities variables
   */
  function dump() {
    return array(
      'layers' => $this->layers,
      'styles' => $this->styles,
      'filetypes' => $this->filetypes,
      'crs' => $this->crs,
      'exceptions' => $this->exceptions,
      'identification' => $this->identification,
      'bounds' => $this->bounds, // possibly bad
      'url' => $this->url);
  }

  /**
   * The raw nicemap compatibilities parsing function
   * This function is generalized and contains no Drupal-
   * specific code.
   *
   * Queries the WMS server, parses the XML returned, and 
   * saves the relevant information in this object.
   * @param $base_url the base URL of the WMS server
   * @return array on success, exception on error
   */
  function load_spec($base_url) {
    if(strpos(strtolower($base_url), 'request=getcapabilities')) {
      throw new Exception(' Check that the server URL does not include a WMS query already.');
    }
    
    if(strpos($base_url, '?')) {
      $capabilities = $base_url .'&request=GetCapabilities&service=WMS';
    }
    else {
      $capabilities = $base_url .'?request=GetCapabilities&service=WMS';
    }

    if (!function_exists('simplexml_load_file')) {
      throw new Exception('SimpleXML is required for Nice Map. Please upgrade to PHP5.');
    }

    // Supress errors caused by offline servers
    $cxml = @simplexml_load_file($capabilities);


    if (!$cxml) {
      throw new Exception('The WMS server could not be reached.');
    }

    if ($cxml->ServiceException) {
      throw new Exception('There was a WMS server exception: '.$cxml->ServiceException);
    }

    /*
     * Capabilities metadata
     */

    if ($cxml->Capability->Request->GetMap->Format) {
      foreach ($cxml->Capability->Request->GetMap->Format as $f) {
        $file_types[] = (String) $f;
      }
    }

    if ($cxml->Capability->Exception->Format) {
      foreach ($cxml->Capability->Exception->Format as $e) {
        $file_types[] = (String) $e;
      }
    }

    if ($cxml->Capability->Request) {
      foreach ($cxml->Capability->Request as $r) {
        $operations[] = (String) $r;
      }
    }

    /*
     * ServiceProvider metadata
     */
    $service['organization'] = (String) @$cxml->Service->ContactInformation->
      ContactPersonPrimary->ContactOrganization;
    if($cxml->Service->OnlineResource) {
      $online_resource = $cxml->Service->OnlineResource->attributes('xlink', true);
      $service['url'] =      (String) $online_resource['href'];
    }
    $service['contact'] =  (String) $cxml->Service->ContactInformation;
    $service['name'] =     (String) $cxml->Service->Name;
    $service['title'] =    (String) $cxml->Service->Title;
    $service['abstract'] = (String) $cxml->Service->Abstract;

    // Base elements
    $this->identification =    $service;
    $this->filetypes  =  $file_types;
    $this->operations = $operations;
    $this->exceptions = $exceptions;

    if ($spec_layers = $cxml->Capability->Layer->Layer) {
      // Master layer info
      if (isset($cxml->Capability->Layer->CRS)) {
        foreach($cxml->Capability->Layer->CRS as $c) {
          $this->crs[] = (String) $c;
        }
      }
      // Grab information particular to each layer
      foreach ($spec_layers as $l) {
        $layers[(String) $l->Name] = array(
          'title' => (String) $l->Title,
          'bounds' =>
            array(
              'minx' => (float) $l->LatLonBoundingBox['minx'],
              'miny' => (float) $l->LatLonBoundingBox['miny'],
              'maxx' => (float) $l->LatLonBoundingBox['maxx'],
              'maxy' => (float) $l->LatLonBoundingBox['maxy']),
          );
          if ((String) $l->SRS) {
            $layers[(String) $l->Name]['srs'] =  (String) $l->SRS;
          }

          if ((String) $l->CRS) {
            foreach($l->CRS as $lcrs) {
              $layers[(String) $l->Name]['srs'] =  (String) $lsrs;
            }
          }

          if ($l->Style) {
            foreach ($l->Style as $style) {
              $layers[(String) $l->Name]['styles'][(String) $style->Name] = 
                (String) $style->Title;
            }
            // Sort styles
            ksort($layers[(String) $l->Name]['styles']);
          }
      }
    }
    $this->layers = $layers;
  }

   /**
   * Expand map to fit all of the given points
   * This is essentially the inverse of process()
   * instead of cutting points because they won't fit, it
   * expands the map so all points fit.
   * @param $items array of points
   * @return none
   * TODO: custom padding / zoom?
   */
  function expand($items, $target) {
    // Preserve original map bounds.
    // Common 'boundary case' in which there are no
    // preexisting bounds, so you need to just use the first element to
    // prime the pump
    if(!$this->bounds['miny']) {
      $this->bounds['miny'] = $items[0]['lat'] - 5;
      $this->bounds['maxy'] = $items[0]['lat'] + 5;
      $this->bounds['minx'] = $items[0]['lon'] - 5;
      $this->bounds['maxx'] = $items[0]['lon'] + 5;
    }

    foreach ($items as $item) {
      // If any of the items are outside the box
      if (
        $item['lat'] < $this->bounds['miny'] ||
        $item['lat'] > $this->bounds['maxy'] ||
        $item['lon'] < $this->bounds['minx'] ||
        $item['lon'] > $this->bounds['maxx']
      ) {
        //TODO: fix padding
        $this->bounds['miny'] = min($item['lat'], $this->bounds['miny']) - 10;
        $this->bounds['maxy'] = max($item['lat'], $this->bounds['maxy']) + 10;
        $this->bounds['minx'] = min($item['lon'], $this->bounds['minx']) - 10;
        $this->bounds['maxx'] = max($item['lon'], $this->bounds['maxx']) + 10;
      }
    }
  }

  /**
   * Expand or contract map as needed given coordinates 
   * and the desired pixel size
   *
   * @param $items
   *   Array of items to display on the map
   * @param $target
   *   Array, target size of map
   *
   * @return
   *   Trimmed array of items to display on the map.
   */
  function process($items, $target = array(), $projection) {
    // This can be called without a projection being set
    // so we need to check that the projection object exists
    $this->projection = nicemap_get_projection($projection);
    if($this->projection) {
      // Preserve original map bounds.
      $orig_map = $this->bounds;
      $this->bounds = $this->projection->getmap($this->bounds, $target);

      $points = array();
      foreach ($items as $item) {
        if (
          $item['lat'] > $orig_map['miny'] &&
          $item['lat'] < $orig_map['maxy'] &&
          $item['lon'] > $orig_map['minx'] &&
          $item['lon'] < $orig_map['maxx']
        ) {
          list($x, $y) = $this->projection->getpoint($this->bounds, $item);
          $ratio['y'] = $y / $this->bounds['h'];
          $ratio['x'] = $x / $this->bounds['w'];
          $y = 100 - (100 * $ratio['y']);
          $x = 100 * $ratio['x'];
          $item['x'] = $x;
          $item['y'] = $y;
          $points[] = $item;
        }
      }
      return $points;
    }
    else {
      throw new Exception('A projection must be set before processing points.');
    }
  }

  /**
   * Generate a new map URL
   * @param $overrides Option overrides, which are absolute.
   * Valid keys are:
   * - styles
   * - bgcolor
   * - request
   * - version
   * - format
   * - layers,
   * - srs
   * - EXCEPTIONS
   * - bbox
   * @return $url string
   */
  function url($overrides = array()) {
    // Options defaults
    $options['request'] = 'GetMap';
    $options['version'] = '1.1.1';
    $options['format'] = 'image/png';

    $options['srs'] = 'EPSG:900913';
    $options['EXCEPTIONS'] = 'application/vnd.ogc.se_inimage';
    $options['bbox'] = $this->bounds['minx'] .','. $this->bounds['miny']
      .','. $this->bounds['maxx'] .','. $this->bounds['maxy'];

    // Override options with passed array
    foreach($overrides as $k => $v) {
      // Handle arrays (styles, layers)
      if(is_array($v)) {
        $v = implode(',', $v);
      }
      $options[$k] = $v;
    }

    if($overrides['bgcolor']) {
      $options['bgcolor'] = '0x'. $overrides['bgcolor'];
    }

    foreach ($options as $key => $val) {
      $q[] = $key."=".$val;
    }
    $query_string = implode($q, "&");

    if(strpos($this->url, '?')) {
      return $this->url ."&". $query_string;
    }
    else {
      return $this->url ."?". $query_string;
    }
  }
}

/**
 * PROJECTION HELPERS =======================================
 */

/**
 * Helper function that provides a new projection object.
 * @param $type string, either 'equirectangular' or 'mercator'
 * @return projection object
 */
function nicemap_map_projection($type) {
  switch ($type) {
    case 'equirectangular':
      $projection = new nicemap_equirectangular_projection();
      return $projection;
    case 'mercator':
      $projection = new nicemap_mercator_projection();
      return $projection;
  }
  return FALSE;
}

/**
 * Interface for nicemap map projections.
 */
interface nicemap_projection {
  /**
   * Get map takes a boundry array and a target size 
   * and generates usable coordinates.
   *
   * @param $map
   * @param $target
   *
   * @return
   *   a treated map array
   */
  public function getmap($map, $target);

  /**
   * Get point plots a point on a map.
   *
   * @param $map
   * @param $item
   *
   * @return
   *   a two element array of x, y.
   */
  public function getpoint($map, $item);
}

/**
 * Provides a mercator projection for nicemap.
 */
class nicemap_mercator_projection implements nicemap_projection {

  public function getmap($map, $target) {
    // create height in terms of mercator projection
    // and set an origin height coordinate
    $map['w'] = deg2rad($map['maxx']) - deg2rad($map['minx']);
    $map['h'] = asinh(tan(deg2rad($map['maxy']))) - asinh(tan(deg2rad($map['miny'])));
    $map['o'] = asinh(tan(deg2rad($map['miny'])));
    $map['ratio'] = $map['w'] / $map['h'];

    if (count($target)) {
      $target['ratio'] = $target['width'] / $target['height'];
      // target is wider than map
      if ($target['ratio'] >= $map['ratio']) {
        // calculate new width from ratio
        $new_w = $map['h'] * $target['ratio'];

        // Adjust longitudes

        // We resize the map sides in radians
        $map['minx'] = deg2rad($map['minx']) - ($new_w - $map['w']) * .5;
        $map['maxx'] = deg2rad($map['maxx']) + ($new_w - $map['w']) * .5;

        // Check to see whether the boundaries are past +-180deg 
        // bounds (pi radians)
        // If they are, pull it in to 180deg and compensate the 
        // difference on the other bound.

        // If both bounds are over the limit, we set to -180 to 
        // 180 -- at least the map will not totally break...
        if (($map['minx'] < (M_PI * -1)) && ($map['maxx'] > M_PI)) {
          $map['minx'] = M_PI * -1;
          $map['maxx'] = M_PI;
        }
        else if ($map['minx'] < (M_PI * -1)) {
          $diff = $map['minx'] - (M_PI * -1);
          $map['minx'] = $map['minx'] - $diff;
          $map['maxx'] = $map['maxx'] - $diff;
        }
        else if ($map['maxx'] > M_PI) {
          $diff = ($map['maxx'] - M_PI);
          $map['maxx'] = $map['maxx'] - $diff;
          $map['minx'] = $map['minx'] - $diff;
        }

        // Convert bounds to degrees
        $map['miny'] = rad2deg($map['miny']);
        $map['maxy'] = rad2deg($map['maxy']);

        // overwrite width + ratio
        $map['w'] = $new_w;
        $map['ratio'] = $target['ratio'];
      }
      // target is taller than map
      else {
        // 1. we know the map is wide enough
        // 2. we need to find the necessary radian height to match ratios
        // 3. once we have the radian height, we need to 
        // adjust the bounding latitudes
        // @TODO: make sure that we don't adjust past the poles...

        // subtract half the height from top to find midpoint
        $midpoint = asinh(tan(deg2rad($map['maxy']))) - ($map['h']*.5);

        // set map height to match target height
        $map['h'] = $map['w'] / $target['ratio']; // 2

        // find origin in radians
        $map['o'] = $midpoint - ($map['h']*.5);

        // find new latitude bounds by converting radian bounds to mercator degrees
        $map['maxy'] = rad2deg(atan(sinh($midpoint + ($map['h']*.5))));
        $map['miny'] = rad2deg(atan(sinh($midpoint - ($map['h']*.5))));
      }
    }
    $map['ratio'] = $map['w'] / $map['h'];

    return $map;
  }

  public function getpoint($map, $item) {
    $y = asinh(tan(deg2rad($item['lat']))) - $map['o'];
    $x = deg2rad($item['lon']) - deg2rad($map['minx']);

    return array($x, $y);
  }
}

/**
 * Provides a equirectangular projection for nicemap.
 *
 * Simply convert all degrees into radians and press "ALL SYSTEMS GO"
 *
 * This class is generalized. Should be safe for external libraries.
 * TODO test!
 */
class nicemap_equirectangular_projection implements nicemap_projection {

  /**
   * @param $map a map array in the making?
   * @param $target target size / aspect ratio
   * @return map array with new coordinates
   */
  public function getmap($map, $target) {
    $map['w'] = deg2rad($map['maxx']) - deg2rad($map['minx']);
    $map['h'] = deg2rad($map['maxy']) - deg2rad($map['miny']);

    $map['o'] = deg2rad($map['miny']);
    $map['ratio'] = $map['w'] / $map['h'];

    if (count($target)) {
      $target['ratio'] = $target['width'] / $target['height'];
      // target is wider than map
      if ($target['ratio'] >= $map['ratio']) {

        // calculate new width from ratio
        $new_w = $map['h'] * $target['ratio'];

        // adjust longitudes
        $map['minx'] = $map['minx'] - rad2deg(($new_w - $map['w']) * .5);
        $map['maxx'] = $map['maxx'] + rad2deg(($new_w - $map['w']) * .5);
        $map['minx'] = ($map['minx'] < -180) ? $map['minx'] = -179.99 : $map['minx'];
        $map['maxx'] = ($map['maxx'] >  180) ? $map['maxx'] =  179.99 : $map['maxx'];

        // overwrite width + ratio
        $map['w'] = $new_w;
        $map['ratio'] = $target['ratio'];
      }
      // target is taller than map
      else {
        // 1. we know the map is wide enough
        // 2. we need to find the necessary radian height to match ratios
        // 3. once we have the radian height, we need to adjust the bounding latitudes

        // subtract half the height from top to find midpoint
        $midpoint = $map['maxy'] - (rad2deg($map['h'])*.5);

        // set map height to match target height
        $map['h'] = $map['w'] / $target['ratio']; // 2

        // find origin in radians
        $map['o'] = deg2rad($midpoint) - ($map['h']*.5);

        // find new latitude bounds by converting radian bounds to mercator degrees
        $map['maxy'] = $midpoint + rad2deg($map['h']*.5);
        $map['miny'] = $midpoint - rad2deg($map['h']*.5);
      }
    }
    $map['ratio'] = $map['w'] / $map['h'];
    return $map;
  }

  public function getpoint($map, $item) {
    $x = deg2rad($item['lon']) - deg2rad($map['minx']);
    $y = deg2rad($item['lat']) - $map['o'];

    return array($x, $y);
  }
}


/**
 * Helper function that determines the correct projection.
 * @param $p The EPSG code of a projection, like EPSG:4326
 * @return new projection object
 * This should probably give an exception instead of
 * returning false?
 */
function nicemap_get_projection($p) {
  $projections = nicemap_projections();
  if ($p && isset($projections[$p])) {
    return new $projections[$p]['class']();
  }
  return FALSE;
}

/**
 * Provides an array of projection options -- usable for form selects.
 * @param none
 * @return array of projections.
 * Why isn't this just an array
 */
function nicemap_projections() {
  return array(
    'EPSG:4326' => array(
      'name' => t('Equirectangular'),
      'class' => 'nicemap_equirectangular_projection',
    ),
    'EPSG:900913' => array(
      'mercator' => t('Google Mercator'),
      'class' => 'nicemap_mercator_projection', // Don't have a definition yet, so we fall back.
    ),
  );
}

