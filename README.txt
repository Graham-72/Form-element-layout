
TABLE OF CONTENTS
================================================================================
* ABOUT
* MODULES
  - Form element layout (fel.module)
  - Form element layout fields (fel_fields.module)
* HOW IT WORKS: THEME REPLACEMENTS
  - Core replacements
  - Contrib replacements
* RELATED PROJECTS AND ISSUES


ABOUT
================================================================================

Override the order of title, description and input elements in forms.

Fields are supported, which allows you to move the help text (#description)
before the input element, configurable on a per field basis. This is useful for
multi value fields or fields with a large input space where the help text falls
far away from the field title.

At the most primitive level it is an Form API extension that overrides the order
of #title, #description and #children in form elements by manipulating the new
#description_display and the existing #title_display attribute.


MODULES
================================================================================

Form element layout (fel.module)
--------------------------------------------------------------------------------

API only module. Doesn't modify or alter anything by itself. It's a thin
extension to Form API with an additional behavior around the
'#description_display' attribute. It behaves much like the issue for D8 (see
below). It is intended for module developers and advanced themers who want to
manually alter forms.

This module replaces the theming of individual form elements with a modified
version (theme('form_element') → theme('fal_form_element'). And because it takes
over the entire rendering it basically controls the layout of the primitive form
elements.

Usage for developers/themers:

    $form['example'] = array(
      '#type' => 'textfield',
      '#title' => t("Example input"),
      '#description' => t("Example description of what this element does."),
      '#description_display' => 'before', // Default is 'after'.
    );

Form element layout fields (fel_fields.module)
--------------------------------------------------------------------------------

Fields support for Form element layout. What makes this package useful for end
users. Configure the position of the help text
($field['instance']['description']) for fields to be either before or after the
input element. This is done in the field UI's field edit form.

It has support for all field types core provides in addition to some popular
contrib field modules:
* Date
* Email field
* Entity reference
* Field Collection Table
* Geofield
* Matrix
* Media
* Meta tags quick
* Multiupload filefield widget
* Multiupload imagefield widget

If your favorite module isn't supported, please issue a feature request for it!


HOW IT WORKS: THEME REPLACEMENTS
================================================================================

In order to alter the order of field elements a few themes need to be altered.
Instead of hijacking existing themes through hook_theme_registry_alter(), this
module will as far as possible use replacement themes. That is, replace
$element['#theme'] or $element['#theme_wrapper'][] with our own version.

This makes it still possible for themers to replace or further alter the themes
by overriding the themes while keeping the functionality provided by this
module. The replacement functions are:

Core replacements
--------------------------------------------------------------------------------

  +------------------------------------------------------+
  | Original theme            | Replacement theme        |
  |---------------------------+--------------------------|
  | form_element              | fel_form_element         |
  | text_format_wrapper       | fel_text_format_wrapper  |
  | field_multiple_value_form | fel_fields_multiple_form |
  +------------------------------------------------------+

Contrib replacements
--------------------------------------------------------------------------------

  +-----------------------------------------------------------------------------------------------------+
  | Module                 | Original theme                               | Replacement theme           |
  |------------------------+----------------------------------------------+-----------------------------|
  | Field collection table | field_collection_table_multiple_value_fields | fel_fields_collection_table |
  | Matrix                 | matrix_table                                 | fel_fields_matrix_table     |
  +-----------------------------------------------------------------------------------------------------+
