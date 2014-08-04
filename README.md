
## Table of contents

* About
* Modules in package
    - Form element layout (fel.module)
    - Form element layout fields (fel_fields.module)
* How it works: Theme replacements
    - Core replacements
    - Contrib replacements
* Related projects and issues


## About

Override the order of title, description and input elements in forms.

Fields are supported, which allows you to move the help text (`#description`)
before the input element, configurable on a per field basis. This is useful for
multi value fields or fields with a large input space where the help text falls
far away from the field title.

At the most primitive level it is an Form API extension that overrides the order
of `#title`, `#description` and `#children` in form elements by manipulating the
new `#description_display` and the existing `#title_display` attribute.


## Modules in package

### Form element layout (fel.module)

API only module. Doesn't modify or alter anything by itself. It's a thin
extension to Form API with an additional behavior around the
`#description_display` attribute. It behaves much like the issue for D8 (see
below). It is intended for module developers and advanced themers who want to
manually alter forms.

This module replaces the theming of individual form elements with a modified
version (`theme('form_element')` → `theme('fel_form_element')`). And because it
takes over the entire rendering it basically controls the layout of the
primitive form elements.

Usage for developers/themers:

    $form['example'] = array(
      '#type' => 'textfield',
      '#title' => t("Example input"),
      '#description' => t("Example description of what this element does."),
      '#description_display' => 'before', // Default is 'after'.
    );


### Form element layout fields (fel_fields.module)

Fields support for Form element layout. What makes this package useful for end
users. Configure the position of the help text
(`$field['instance']['description']`) for fields to be either before or after
the input element. This is done in the field UI's field edit form.

It has support for all field types core provides in addition to some popular
contrib field modules:

* [Date                         ](https://www.drupal.org/project/date)
* [Email field                  ](https://www.drupal.org/project/email)
* [Entity reference             ](https://www.drupal.org/project/entityreference)
* [Field Collection Table       ](https://www.drupal.org/project/field_collection_table)
* [Geofield                     ](https://www.drupal.org/project/geofield)
* [Matrix                       ](https://www.drupal.org/project/matrix)
* [Media                        ](https://www.drupal.org/project/media)
* [Meta tags quick              ](https://www.drupal.org/project/metatags_quick)
* [Multiupload filefield widget ](https://www.drupal.org/project/multiupload_filefield_widget)
* [Multiupload imagefield widget](https://www.drupal.org/project/multiupload_imagefield_widget)

If your favorite module isn't supported, please issue a feature request for it!


## How it works: theme replacements

In order to alter the order of field elements a few themes need to be altered.
Instead of hijacking existing themes through `hook_theme_registry_alter()`, this
module will as far as possible use replacement themes. That is, replace
`$element['#theme']` or `$element['#theme_wrapper'][]` with our own version.

This makes it still possible for themers to replace or further alter the themes
by overriding the themes while keeping the functionality provided by this
module. The replacement functions are:


### Core replacements

Original theme                | Replacement theme
------------------------------|--------------------------
[form_element][]              | fel_form_element
[text_format_wrapper][]       | fel_text_format_wrapper
[field_multiple_value_form][] | fel_fields_multiple_form

[form_element]:              https://api.drupal.org/api/drupal/includes!form.inc/function/theme_form_element/7
[text_format_wrapper]:       https://api.drupal.org/api/drupal/modules!filter!filter.module/function/theme_text_format_wrapper/7
[field_multiple_value_form]: https://api.drupal.org/api/drupal/modules!field!field.form.inc/function/theme_field_multiple_value_form/7


### Contrib replacements

Module                 | Original theme                               | Replacement theme
-----------------------|----------------------------------------------|-----------------------------
Field collection table | field_collection_table_multiple_value_fields | fel_fields_collection_table
Matrix                 | matrix_table                                 | fel_fields_matrix_table


## Related projects and issues

* [D8 issue: #314385](https://www.drupal.org/node/314385)
* D7 sandbox module: [Top Description][]. This module moves all element
  descriptions above the input element. For all forms all over. No settings to
  modify its behavior. Minimally maintained. Maintenance fixes only.
* D7 module: [Label Help][]. This module adds an additional text element that
  are displayed before the input element. It doesn't touch or move the existing
  field $instance['description'].
* D7 module: [Extra Field Description][]. Behaves exactly like Label Help.
* D7 module: [Better field description][]. Like the two above it adds an
  additional text for fields, not touching the original help text
  (`$instance['description']`). The twist here is that it extracts the editing
  of the text to a different page with different permission, allowing non-admins
  (like, say, editors) to edit it without messing up other field settings.
* D6 sandbox module: [Element shuffle][]. This the first version of Form element
  layout. The concept is the same, but was not suitable for D7 port and had some
  real context leakage. FEL was therefore rewritten from scratch.

[top description]:          https://www.drupal.org/sandbox/jrb/top_description
[label help]:               https://www.drupal.org/project/label_help
[extra field description]:  https://www.drupal.org/project/extra_field_description
[better field description]: https://www.drupal.org/project/better_field_descriptions
[element shuffle]:          https://www.drupal.org/sandbox/kaare/1132056
