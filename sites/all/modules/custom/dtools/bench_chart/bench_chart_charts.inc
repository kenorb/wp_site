<?php

/**
 * Chart reports page
 */
function bench_chart_charts() {
    $output = '';

    $last_arg = end(arg()); // check for last argument (custom chart request)
    if (strlen($last_arg) == 32) {
        $path = db_result(db_query('SELECT path FROM {bench_chart} WHERE md5(path) = \'%s\' AND count > 0', $last_arg));
        $chart = _charts_system_generate(
            t('Hook execution time for path: @path', array('@path' => $path)),
            "SELECT CONCAT(ROUND(AVG(totaltime)/1000,2),'s') AS count, hook AS name
            FROM {bench_chart}
            WHERE path = '$path'
            GROUP BY hook
            ORDER BY count DESC
            LIMIT 10",
            'pie3D'
        );
        $output .= "<div id='charts-chart'>$chart</div>";
        $chart = _charts_system_generate(
            t('Hook execution count for path: @path', array('@path' => $path)),
            "SELECT count AS count, hook AS name
            FROM {bench_chart}
            WHERE path = '$path'
            GROUP BY hook
            ORDER BY count DESC
            LIMIT 10",
            'pie3D'
        );
        $output .= "<div id='charts-chart'>$chart</div>";
    } else  {
        $res_paths = db_query('SELECT path FROM {bench_chart} WHERE count = 0 GROUP BY path ORDER BY totaltime DESC LIMIT 3');
        if ($res_paths) {
            while ($path = db_fetch_object($res_paths)) {
                $path->path = db_escape_string($path->path);
                $chart = _charts_system_generate(
                    t('Execution time of hook modules for path: @path', array('@path' => $path->path)),
                    'SELECT CONCAT(ROUND(AVG(time)/1000,2),"s") AS count, hook AS name
                    FROM {bench_chart}
                    GROUP BY hook
                    ORDER BY time DESC
                    LIMIT 10',
                    'pie3D'
                );
                $output .= "<div id='charts-chart'>$chart</div>";
            }
        }
    }

        $chart = _charts_system_generate(
            t('Average hook execution time'),
            'SELECT CONCAT(ROUND(AVG(time)/1000,2),"s") AS count, hook AS name
            FROM {bench_chart}
            WHERE count > 0
            GROUP BY hook
            ORDER BY count DESC
            LIMIT 10',
            'pie3D'
        );
    $output .= "<div id='charts-chart'>$chart</div>";

        $chart = _charts_system_generate(
            t('Average hook execution count'),
            'SELECT count AS count, hook AS name
            FROM {bench_chart}
            WHERE count > 0
            GROUP BY hook
            ORDER BY count DESC
            LIMIT 10',
            'pie3D'
        );
    $output .= "<div id='charts-chart'>$chart</div>";

    $res_paths = db_query('SELECT path FROM {bench_chart} WHERE count > 0 GROUP BY path');
    if ($res_paths) {
	while ($path = db_fetch_object($res_paths)) {
            $path->path = db_escape_string($path->path);
            $chart = _charts_system_generate(
                t('Execution time for path: @path', array('@path' => $path->path)),
                "SELECT CONCAT(ROUND(AVG(totaltime)/1000,2),'s') AS count, hook AS name
                FROM {bench_chart}
                WHERE path = '$path->path' AND count > 0
                GROUP BY hook
                ORDER BY totaltime",
                'line2D'
            );
            $output .= "<div id='charts-chart'>$chart</div>";

            $path_md5 = md5($path->path);
            $gen_url = "admin/reports/charts/hook_stat/$path_md5";
            $text = t('Generate hook stats for page: @path', array('@path' => $path->path));
            $text = t('Generate module stats for page: @path', array('@path' => $path->path));
            $output .= '<br><br>'.l($text, $gen_url);
	}
    }

  return '<div id="charts-system">'. $output .'</div>';
}

/**
 * Generate some charts using a pre defined method.
 *
 * @param $title
 *   String. The chart title
 * @param $sql
 *   String. The SQL statement to be executed
 * @param $callback
 *   String (optional). When a string is given, use it as the
 *   parser of the results from SQL. Its important when the
 *   results are coded in to DB to ocupy less space, and should
 *   be decoded.
 * @return
 *   String. The HTML chart when all data is fine or a blank string
 */
function _charts_system_generate($title, $sql, $type, $callback = NULL) {
  $results = db_query($sql);

  while ($result = db_fetch_array($results)) {
    if (!empty($callback)) {
      $result['name'] = $callback($result['name']);
    }
    switch ($type) {
        case 'line2D':
            $data[] = array(
              '#value'  => $result['count'],
              '#label'  => $result['name']
            );
            break;
        default:
            $data[] = array(
              '#value'  => $result['count'],
              '#label'  => $result['name'] .': '. $result['count']
            );
        break;
    }
  }

  if (!empty($data)) {
    $settings = variable_get('charts_settings', array());
    $chart[0] = $data;
    $chart['#title']    = $title;
    if ($type) {
	$chart['#type']     = $type;
	$chart['#width']     = empty($settings['#width']) ? 600 : $settings['#width'];
	$chart['#height']     = empty($settings['#height']) ? 200 : $settings['#height'];
    }

    return charts_chart($chart);
  }

  return '';
}

/**
 * menu()'s callback
 */
function bench_chart_show_form() {
    $example = array(
	'#type'     => 'pie3D', // Display a 3D pie chart
	'#color'    => 'f0f0f0', // Background color, in RRGGBB format
	'#title'    => t('Example'), // Chart title

	array(5, 10, 25, 60),
    );
$example = array(
  '#type'     => 'pie3D', // Display a 3D pie chart
  '#color'    => 'f0f0f0', // Background color, in RRGGBB format
  '#title'    => t('Example'), // Chart title
  array(
    array('#value' => 60, '#label' => t('60%')),
    array('#value' => 25, '#label' => t('25%')),
    array('#value' => 10, '#label' => t('10%')),
    array('#value' => 5,  '#label' => t('5%')),
  ),
);
    return charts_chart($example);
}


