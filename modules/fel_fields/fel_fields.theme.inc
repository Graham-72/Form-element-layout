<?php

/**
 * @file
 * Theme replacement functions for Form element layout fields.
 */

/**
 * Theme replacement for 'date_combo'.
 *
 * @see http://drupalcontrib.org/api/drupal/contributions!date!date.theme/function/theme_date_combo/7
 */
function theme_fel_date_combo($variables) {
  $element = $variables['element'];

  // Group start/end items together in fieldset.
  $fieldset = array(
    '#title' => field_filter_xss(t($element['#title'])) . ' ' . ($element['#delta'] > 0 ? intval($element['#delta'] + 1) : ''),
    '#value' => '',
    '#attributes' => array(),
    '#children' => $element['#children'],
  );
  if (!empty($element['#fieldset_description'])) {
    $fieldset['#description'] = $element['#fieldset_description'];
    if (!empty($element['#description_display'])) {
      $fieldset['#description_display'] = $element['#description_display'];
    }
    if (!empty($element['#description_classes'])) {
      $fieldset['#description_classes'] = $element['#description_classes'];
    }
  }
  if (!empty($element['#title_classes'])) {
    $fieldset['#title_classes'] = $element['#title_classes'];
  }
  // Add marker to required date fields.
  if ($element['#required']) {
    $fieldset['#title'] .= " " . theme('form_required_marker');
  }
  return theme('fel_fieldset', array('element' => $fieldset));
}

/**
 * Theme replacement for 'field_collection_table_multiple_value_fields'.
 *
 * Which again is an override for field_multiple_value_form.
 *
 * @see theme_field_collection_table_multiple_value_fields()
 */
function theme_fel_fields_collection_table($variables) {
  $element = $variables['element'];

  if (isset($element['#cardinality']) && ($element['#cardinality'] > 1 || $element['#cardinality'] == FIELD_CARDINALITY_UNLIMITED)) {
    $table_id = drupal_html_id($element['#field_name'] . '_values');
    $order_class = $element['#field_name'] . '-delta-order';
    $required = !empty($element['#required']) ? theme('form_required_marker', $variables) : '';
    $draggable = empty($element['#custom_settings']['nodragging']);
    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation).
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

    $header = $draggable ? array(array('data' => '', 'class' => '')) : array();
    // Add the items as table rows.
    foreach ($items as $key => $item) {
      uasort($item, 'element_sort');
      $item['_weight']['#attributes']['class'][] = $order_class;
      $cells = array();
      if ($draggable) {
        $cells[] = array('data' => '', 'class' => array('field-multiple-drag'));
      }
      foreach (element_children($item) as $field_name) {
        if ($draggable || $field_name != '_weight') {
          // Only add the header once.
          if ($key == 0) {
            $header[] = array(
              'data' => '<label>' . t('!title', array('!title' => _field_collection_table_get_title($item[$field_name]))) . '</label>',
              'class' => array('field-label'),
            );
          }
          $cells[] = array('data' => $item[$field_name]);
        }
      }
      $row = array('data' => $cells);
      if ($draggable) {
        $row['class'] = array('draggable');
      }
      $rows[] = $row;
    }

    $element['#title_classes'][] = 'field-label';
    $items['title'] = '<label for="' . $table_id . '" class="' . implode(' ', $element['#title_classes']) . '">';
    $items['title'] .= t('!title !required', array('!title' => $element['#title'], '!required' => $required));
    $items['title'] .= '</label>';

    if (!empty($element['#description'])) {
      $items['description'] = theme('fel_form_element_description', $variables);
    }
    $items['children'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-multiple-table'))));

    $output = '<div class="form-item">';
    $output .= fel_order_output($element, $items);
    $output .= '</div>';

    if (isset($add_more_button)) {
      $output .= drupal_render($add_more_button) . '<div class="clearfix"></div>';
    }

    if ($draggable) {
      drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
    }
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

/**
 * Theme replacement for 'matrix_table'.
 *
 * @see theme_matrix_table()
 */
function theme_fel_fields_matrix_table($variables) {
  $form = $variables['form'];
  $rows_count = $form['#matrix_rows'];
  $cols_count = $form['#matrix_cols'];
  $more_cols = $form['#matrix_more_cols'];
  $more_rows = $form['#matrix_more_rows'];
  $column_labels = $form['#column_labels'];
  $row_labels = $form['#row_labels'];
  $field_name = $form['#field_name'];

  $table_rows = array();
  for ($row = 1; $row <= $rows_count; $row++) {
    for ($col = 1; $col <= $cols_count; $col++) {
      $table_rows[$row - 1][$col - 1] = drupal_render($form['grid'][$row . '-' . $col]);
    }
  }

  if (!empty($row_labels)) {
    array_unshift($column_labels, '');
    foreach ($row_labels as $id => $label) {
      array_unshift($table_rows[$id - 1], $label);
    }
  }

  if ($more_rows) {
    $table_rows[$rows_count][1] = drupal_render($form['more_rows']);
    for ($col = 2; $col <= $cols_count; $col++) {
      $table_rows[$rows_count][$col] = '&nbsp;';
    }
  }

  if ($more_cols) {
    $column_labels[] = drupal_render($form['more_cols']);
    for ($row = 1; $row <= $rows_count; $row++) {
      $table_rows[$row - 1][$cols_count + 1] = '&nbsp;';
    }
  }

  $parts = array(
    'title' => theme('fel_form_element_label', (array('element' => $form))),
    'children' => theme('table', array('header' => $column_labels, 'rows' => $table_rows)),
  );
  if (!empty($form['#description'])) {
    $form['#description'] = filter_xss_admin($form['#description']);
    $parts['description'] = theme('fel_form_element_description', array('element' => $form));
  }
  $output  = '<div id="matrix-field-' . $field_name . '">';
  $output .= fel_order_output($form, $parts);
  $output .= '</div>';
  return $output;
}

/**
 * Theme replacement for 'multipage_pane' from field_group.module.
 *
 * @see theme_multipage_pane()
 */
function theme_fel_fields_multipage_pane($variables) {

  $element = $variables['element'];
  $group = $variables['element']['#group_object'];
  $parent_group = $variables['element']['#parent_group_object'];

  static $multipages;
  if (!isset($multipages[$group->parent_name])) {
    $multipages = array($group->parent_name => 0);
  }
  $multipages[$parent_group->group_name]++;

  // Create a page title from the label.
  $page_header = isset($parent_group->format_settings['instance_settings']['page_header']) ? $parent_group->format_settings['instance_settings']['page_header'] : 3;
  switch ($page_header) {
    case 1:
      $title = $element['#title'];
      break;

    case 2:
      $title = t('Step %count of %total', array('%count' => $multipages[$parent_group->group_name], '%total' => count($parent_group->children)));
      break;

    case 3:
      $title = t('Step %count of %total !label', array('%count' => $multipages[$parent_group->group_name], '%total' => count($parent_group->children), '!label' => $element['#title']));
      break;

    case 0:
    default:
      $title = '';
      break;
  }

  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));

  $output = '<div' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<h2 class="multipage-pane-title"><span>' . $title . '</span></h2>';
  }
  $output .= '<div class="fieldset-wrapper multipage-pane-wrapper">';
  if (!empty($element['#description'])) {
    $element['#description_classes'][] = 'fieldset-description';
    $output .= theme('fel_form_element_description', array('element' => $element));
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  // Add a page counter if needed.
  // counter array(0 => t('No'), 1 => t('Format 1 / 10'), 2 => t('The count number only'));
  $page_counter_format = isset($parent_group->format_settings['instance_settings']['page_counter']) ? $parent_group->format_settings['instance_settings']['page_counter'] : 1;
  if ($page_counter_format == 1) {
    $output .= t('<span class="multipage-counter">%count / %total</span>', array('%count' => $multipages[$parent_group->group_name], '%total' => count($parent_group->children)));
  }
  elseif ($page_counter_format == 2) {
    $output .=  t('<span class="multipage-counter">%count</span>', array('%count' => $multipages[$parent_group->group_name]));
  }

  $output .= '</div>';
  $output .= "</div>\n";

  return $output;

}
