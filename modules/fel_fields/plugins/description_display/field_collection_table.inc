<?php

/**
 * @file
 * Plugin 'description_display' for field_collection_table.module.
 */

/**
 * Plugin callback.
 */
function fel_fields_field_collection_table_process_description_display($field, $instance, &$form) {
  $form['#after_build'][] = 'fel_fields_field_collection_table_after_build';
}
