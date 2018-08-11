<?php

/**
 * @file
 * Plugin 'description_display' for text.module.
 */

/**
 * Plugin callback.
 */
function fel_field_matrix_description_display_process($field, $instance, &$form) {
  $form['#theme'] = 'fel_fields_matrix_table';
}
