<?php
// $Id: gallery_service_embed.inc,v 1.1 2008/03/15 18:41:54 profix898 Exp $

/**
 * Function gallery_service_embed_service().
 */
function gallery_service_embed_service() {
  return array(
    // gallery.embed.get_api_version
    array(
      '#method'   => 'gallery.embed.get_api_version',
      '#callback' => 'gallery_service_embed_get_api_version',
      '#auth'     => false,
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Returns the Embedding API version.')
    ),
    // gallery.embed.get_external_id_map
    array(
      '#method'   => 'gallery.embed.get_external_id_map',
      '#callback' => 'gallery_service_embed_get_external_id_map',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'key',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the complete externalId<->entityId map.')
    ),
    // gallery.embed.is_external_id_mapped
    array(
      '#method'   => 'gallery.embed.is_external_id_mapped',
      '#callback' => 'gallery_service_embed_is_external_id_mapped',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'externalId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'entityType',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Checks if externalId is mapped to a G2 user/group..')
    ),
    // gallery.embed.get_image_block
    array(
      '#method'   => 'gallery.embed.get_image_block',
      '#callback' => 'gallery_service_embed_get_image_block',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'params',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns an image block.')
    ),
    // gallery.embed.get_block
    array(
      '#method'   => 'gallery.embed.get_block',
      '#callback' => 'gallery_service_embed_get_block',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'moduleId',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'blockName',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'params',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns a module block.')
    )
  );
}

/* Services wrappers for GalleryEmbed methods */

/**
 * Function gallery_service_embed_get_api_version().
 */
function gallery_service_embed_get_api_version() {
  if (!_gallery_init()) {
    return array();
  }
  
  return GalleryEmbed::getApiVersion();
}

/**
 * Function gallery_service_embed_get_external_id_map().
 */
function gallery_service_embed_get_external_id_map($key) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryEmbed::getExternalIdMap($key);
}

/**
 * Function gallery_service_embed_is_external_id_mapped().
 */
function gallery_service_embed_is_external_id_mapped($externalId, $entityType) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryEmbed::isExternalIdMapped($externalId, $entityType);
}

/**
 * Function gallery_service_embed_get_image_block().
 */
function gallery_service_embed_get_image_block($params) {  
  return gallery_service_embed_get_block('imageblock', 'ImageBlock', $params);
}

/**
 * Function gallery_service_embed_get_block().
 */
function gallery_service_embed_get_block($moduleId, $blockName, $params = array()) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryEmbed::getBlock($moduleId, $blockName, $params);
}
