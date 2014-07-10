
--------------------------------------------------------------------------------
ABOUT

Override the order of title, description and input elements in forms, by
replacing the primitive themes that renders elements.

A frontend for fields is also provided, making it possible to move the
description (Help text) of fields above the input element(s).


--------------------------------------------------------------------------------
MODULES IN PACKAGE

- Form element layout (fel.module)

  API only module. Doesn't modify or alter anything by itself. It's a thin
  extension to Form API with an additional behavior around the
  '#description_display' attribute. It behaves much like the issue for D8 (see
  below). It is intended for module developers and advanced themers who want to
  manually alter forms.

- Form element layout fields (fel_fields.module)

  Fields support for Form element layout. What makes this package useful.
  Configure the position of the help text ($field['instance']['description'])
  for fields to be either before or after the input element. This is done in the
  field UI's field edit form.

See the project homepage for further info and documentation:

    https://www.drupal.org/sandbox/kaare/2292397
