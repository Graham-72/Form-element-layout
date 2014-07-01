<?php

/**
 * Determine the render order of a form element.
 *
 * Returns what order #title, #children and #description should be rendered in,
 * based on #title_display and #description_display.
 *
 * @param array $element
 *   Form element
 *
 * @return array
 *   An array with the three values 'title', 'children' and 'description' in
 *   various orders.
 */
function element_order_get_order($element) {
  if (!empty($element['#element_order'])) {
    return $element['#element_order'];
  }

  $title_before = (empty($element['#title_display']) or $element['#title_display'] == 'before' or $element['#title_display'] == 'invisible');
  $description_before = (empty($element['#description_display']) ? false : ($element['#description_display'] == 'before'));

  if ($title_before) {
    $order = $description_before ? array('title', 'description', 'children') : array('title', 'children', 'description');
  }
  else {
    $order = $description_before ? array('description', 'children', 'title') : array('children', 'title', 'description');
  }
  return $order;
}

/**
 * Return the output of $trio in configured order.
 */
function element_order_output($element, &$parts) {
  $element_order = element_order_get_order($element);
  $parts_ordered = array();
  $out = '';
  foreach ($element_order as $element_item) {
    if (!empty($parts[$element_item])) {
      $out .= $parts[$element_item];
      $parts_ordered[$element_item] = $parts[$element_item];
    }
  }
  $parts = $parts_ordered;
  return $out;
}


/**
 * Theme override for theme('form_element').
 *
 * @see includes/form.inc:theme_form_element()
 */
function element_order_theme_form_element($variables) {
  $element = &$variables['element'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  $element_parts = array('children' => $prefix . $element['#children'] . $suffix);
  if ($element['#title_display'] != 'none' and $element['#title_display'] != 'attribute') {
    $element_parts['title'] = theme('form_element_label', $variables);
  }
  if (!empty($element['#description'])) {
    $element_parts['description'] = '<div class="description">' . $element['#description'] . "</div>\n";
  }
  element_order_output($element, $element_parts);
  $output .= implode(' ', $element_parts);
  $output .= "\n</div>\n";

  return $output;
}

function element_order_theme_text_format_wrapper($variables) {
  $element = $variables['element'];

  $trio['children'] = $element['#children'];
  if (!empty($element['#description'])) {
    $trio['description'] = '<div class="description">' . $element['#description'] . '</div>';
  }
  $output = '<div class="text-format-wrapper">';
  $output .= element_order_output($element, $trio);
  $output .= "</div>\n";

  return $output;
}

/**
 * Theme override for 'field_multiple_value_form'
 *
 * @see modules/field/field.form.inc:theme_field_multiple_value_form().
 *
 */
function element_order_theme_field_multiple_value_form($variables) {
  $element = $variables['element'];
  $output = '';

  if ($element['#cardinality'] > 1 || $element['#cardinality'] == FIELD_CARDINALITY_UNLIMITED) {
    $table_id = drupal_html_id($element['#field_name'] . '_values');
    $order_class = $element['#field_name'] . '-delta-order';
    $required = !empty($element['#required']) ? theme('form_required_marker', $variables) : '';

    $header = array(
      array(
        'data' => '<label>' . t('!title !required', array('!title' => $element['#title'], '!required' => $required)) . "</label>",
        'colspan' => 2,
        'class' => array('field-label'),
      ),
      t('Order'),
    );
    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key === 'add_more') {
        $add_more_button = &$element[$key];
      }
      else {
        $items[] = &$element[$key];
      }
    }
    usort($items, '_field_sort_items_value_helper');

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      $item['_weight']['#attributes']['class'] = array($order_class);
      $delta_element = drupal_render($item['_weight']);
      $cells = array(
        array('data' => '', 'class' => array('field-multiple-drag')),
        drupal_render($item),
        array('data' => $delta_element, 'class' => array('delta-order')),
      );
      $rows[] = array(
        'data' => $cells,
        'class' => array('draggable'),
      );
    }
    $parts['children'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-multiple-table'))));
    if ($element['#description']) {
      $parts['description'] = '<div class="description">' . $element['#description'] . '</div>';
    }
    $output = '<div class="form-item">';
    $output .= element_order_output($element, $parts);
    $output .= '<div class="clearfix">' . drupal_render($add_more_button) . '</div>';
    $output .= '</div>';

    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

/**
 * Theme override for field_collection_table_multiple_value_fields.
 *
 * Which again is an override for field_multiple_value_form().
 */
function element_order_theme_field_collection_table_multiple_value_fields($variables) {
  $element = $variables['element'];
  $output = '';

  if (isset($element['#cardinality']) && ($element['#cardinality'] > 1 || $element['#cardinality'] == FIELD_CARDINALITY_UNLIMITED)) {
    $table_id = drupal_html_id($element['#field_name'] . '_values');
    $order_class = $element['#field_name'] . '-delta-order';
    $required = !empty($element['#required']) ? '<span class="form-required" title="' . t('This field is required. ') . '">*</span>' : '';

    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if (!isset($element[$key]['#entity_type'])) {
        if ($key === 'add_more') {
          $add_more_button = &$element[$key];
        }
      }
      else {
        $items[] = &$element[$key];
      }
    }
    usort($items, '_field_sort_items_value_helper');

    $header = array(
      array(
        'data' => '<label>' . t('!title: !required', array('!title' => $element['#title'], '!required' => $required)) . "</label>",
        'class' => array('field-label'),
      ),
    );

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      uasort($item, 'element_sort');
      $item['_weight']['#attributes']['class'] = array($order_class);
      $cells = array(
        array('data' => '', 'class' => array('field-multiple-drag')),
      );
      foreach (element_children($item) as $field_name) {
        // Only add the header once.
        if ($key == 0) {
          $header[] = array(
            'data' => '<label>' . t('!title', array('!title' => _field_collection_table_get_title($item[$field_name]))) . '</label>',
            'class' => array('field-label'),
          );
        }
        $cells[] = array('data' => $item[$field_name]);
      }
      $rows[] = array(
        'data' => $cells,
        'class' => array('draggable'),
      );
    }

    $output = array(
      '#prefix' => '<div class="form-item">',
      '#suffix' => '</div>',
    );
    if (!empty($element['#description']) and !empty($element['#description_display']) and $element['#description_display'] == 'before') {
      $output[] = array(
        '#prefix' => '<div class="description">',
        '#suffix' => '</div>',
        '#markup' => $element['#description'],
      );
    }
    $output['field_collection_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => array(
        'id' => $table_id,
        'class' => array(
          'field-multiple-table',
        ),
      ),
    );
    if (!empty($element['#description']) and (empty($element['#description_display']) or $element['#description_display'] == 'after')) {
      $output[] = array(
        '#prefix' => '<div class="description">',
        '#suffix' => '</div>',
        '#markup' => $element['#description'],
      );
    }
    if (isset($add_more_button)) {
      $output[] = $add_more_button + array(
        '#prefix' => '<div class="clearfix">',
        '#suffix' => '</div>',
      );
    }

    $output = drupal_render($output);

    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

