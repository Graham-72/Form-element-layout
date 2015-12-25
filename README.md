# Form Element Layout

This module provides Form API and field setting for manipulating
the layout of primitive form elements. Main features:

+ Form attribute for placing the form `#description` before the input 
  element (`#description_display`).
+ Field configuration for moving the help text before or after the input
  element. This is useful for multi value fields or fields with a large 
  input space where the help text falls far away from the field title.
+ Form attribute for adding additional classes to form `#title` and
  `#description`s (`#title_classes` and `#description_classes`).
+ Field labels and help texts have their own CSS classes (`fel-field-label`
  and `fel-field-help-text`) to separate them from additional `#title`s and
  `#description`s added by some field types. This is handy for themers and 
  other modules that need to separate the configured texts from the rest.
  
## Status

This is an initial port of the module. Two functions from ctools have been 
added and one section of code has been omitted in this version.

Functions added:
+ hook_ctools_plugin_type
+ hook_ctools_plugin_directory
+ ctools_include

Omitted:
+ code calling on ctools_get_plugins



## Installation

Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
  
    
## Configuration

No module configuration is needed. 

## API

The main module, *Form element layout* (`fel.module`) provides the primitive
rendering and behavior around the following form attributes:

+ `#description_display`: Where the `#description` should be rendered
  relative to its `#children`. Can be *before* or *after*. Default is 
  *before* for fieldsets and *after* for the rest.
+ `#description_classes`: Array of additional classes to add to the
  `#description` when rendered. The default '*description*' class is 
   always added. It can be further customized by overriding the theme
  `fel_form_element_description`.
+ `#title_classes`: Array of additional classes to add to the `#title`
   when rendered.

Example usage for developers/themers:

    $form['example'] = array(
      '#type' => 'textfield',
      '#title' => t("Example input"),
      '#description' => t("Example description of what this element does."),

      // Attributes honored by fel.module
      '#title_classes' => array('example'),
      '#description_classes' => array('important'),
      '#description_display' => 'before',
    );


## Supported field types

All element and field types core provide is supported by *Form element layout
fields* (`fel_fields.module`). In addition some popular contrib field modules
are supported:

* [Address Field][]
* [Date][]
* [Email field][]
* [Entity reference][]
* [Field Collection Table][]
* [Field Group][]
* [Geofield][]
* [Link][]
* [Matrix][]
* [Media][]
* [Meta tags quick][]
* [Multiupload filefield widget][]
* [Multiupload imagefield widget][]
* [Taxonomy Term Reference Tree Widget][]

Otherwise it has a fall-back mechanism that recursively will attach the
appropriate Form API attributes for elements likely to be rendered. If it
doesn't and your favorite module doesn't respond to the configuration, please
issue a feature request for it!

Field modules may also provide support for `fel_fields.module` by adding a
plugin for it. This is documented in the [Advanced help][] section of this
module.

Support for the Field Group module is only partial in the sense that you cannot
configure or code the position of the group's descriptions, but the description
provided by the user will have its own classes


## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.
    
        
## Current porting to Backdrop

Graham Oliver (github.com/Graham-72/)

## Credits

### Maintainers for Drupal:

- Kåre Slettnes (kaare)


### Acknowledgement

This port to Backdrop would not, of course, be possible without all
the work done by the developers and maintainers of the Drupal module.


