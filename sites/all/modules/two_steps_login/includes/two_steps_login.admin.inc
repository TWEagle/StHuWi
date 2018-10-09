<?php

/**
 * @file
 * Admin settings form and callbacks.
 */

/**
 * Implements hook_settings_form().
 */
function two_steps_login_settings_form($form) {
  $form = array();
  $form[TWO_STEPS_LOGIN_FIELDSET] = array(
    '#type' => 'fieldset',
    '#title' => t('Two steps login configuration'),
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_ENABLE_FORM] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Two steps login'),
    '#default_value' => two_steps_login_is_multi_steps_login_enabled(),
    '#options' => array(
      0 => t('Disable'),
      1 => t('Enable'),
    ),
    '#description' => t('If it is disabled User will see default login form.'),
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_ENABLE_USER_PICTURE] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable User Picture on login form'),
    '#default_value' => two_steps_login_is_user_picture_enable(),
    '#options' => array(
      0 => t('Disable'),
      1 => t('Enable'),
    ),
    '#description' => t('If it is disabled User will not see user picture on login form.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="' . TWO_STEPS_LOGIN_ENABLE_FORM . '"]' => array('checked' => FALSE),
      ),
    ),
    '#validated' => TRUE,
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_PICTURE_FIELDSET] = array(
    '#type' => 'fieldset',
    '#title' => t('User Picture configuration'),
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_PICTURE_FIELDSET][TWO_STEPS_LOGIN_USE_DEFAULT_USER_PICTURE] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default picture field for user'),
    '#default_value' => two_steps_login_use_default_user_picture(),
    '#options' => array(
      0 => t('Disable'),
      1 => t('Enable'),
    ),
    '#description' => t('If it is enabled default user picture will be used on login form.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="' . TWO_STEPS_LOGIN_ENABLE_USER_PICTURE . '"]' => array('checked' => FALSE),
      ),
    ),
    '#validated' => TRUE,
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_PICTURE_FIELDSET][TWO_STEPS_LOGIN_USER_PICTURE_FIELD] = array(
    '#type' => 'textfield',
    '#title' => t('User Picture field'),
    '#default_value' => two_steps_login_get_user_picture_field(),
    '#description' => t('Please enter field machine name e.g. field_example_picture'),
    '#states' => array(
      'invisible' => array(
        ':input[name="' . TWO_STEPS_LOGIN_USE_DEFAULT_USER_PICTURE . '"]' => array('checked' => TRUE),
      ),
    ),
    '#validated' => TRUE,
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_PICTURE_FIELDSET][TWO_STEPS_LOGIN_IMAGE_STYLE] = array(
    '#type' => 'select',
    '#title' => t('Image Style'),
    '#options' => two_steps_login_get_all_image_styles(),
    '#default_value' => two_steps_login_get_image_style(),
    '#description' => t('Please select the image style to be used for user picture on login page'),
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_PICTURE_FIELDSET][TWO_STEPS_LOGIN_DEFAULT_IMAGE] = array(
    '#type' => 'managed_file',
    '#title' => t('Default User Image'),
    '#default_value' => two_steps_login_get_defaut_image(),
    '#description' => t('Please upload default image for user picture to be shown on login form if no image available for user.'),
    '#upload_validators' => array('file_validate_extensions' => array('png jpg gif')),
    '#upload_location' => 'public://',
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_NEXT_BUTTON_NAME] = array(
    '#type' => 'textfield',
    '#title' => t('Next Button Name'),
    '#default_value' => two_steps_login_get_next_button_name(),
    '#description' => t('Please enter next button name.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="' . TWO_STEPS_LOGIN_ENABLE_FORM . '"]' => array('checked' => FALSE),
      ),
    ),
    '#validated' => TRUE,
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_BACK_BUTTON_NAME] = array(
    '#type' => 'textfield',
    '#title' => t('Back Button Name'),
    '#default_value' => two_steps_login_get_back_button_name(),
    '#description' => t('Please enter back button name.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="' . TWO_STEPS_LOGIN_ENABLE_FORM . '"]' => array('checked' => FALSE),
      ),
    ),
    '#validated' => TRUE,
  );
  $form[TWO_STEPS_LOGIN_FIELDSET][TWO_STEPS_LOGIN_LOGIN_BUTTON_NAME] = array(
    '#type' => 'textfield',
    '#title' => t('Login Button Name'),
    '#default_value' => two_steps_login_get_login_button_name(),
    '#description' => t('Please enter login button name.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="' . TWO_STEPS_LOGIN_ENABLE_FORM . '"]' => array('checked' => FALSE),
      ),
    ),
    '#validated' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Use to validate user picture field.
 *
 * @see two_steps_login_settings_form
 */
function two_steps_login_settings_form_validate($form, $form_state) {
  if (!empty($form_state['values'][TWO_STEPS_LOGIN_USER_PICTURE_FIELD])) {
    if (!field_info_instance('user', $form_state['values'][TWO_STEPS_LOGIN_USER_PICTURE_FIELD], 'user')) {
      form_set_error(TWO_STEPS_LOGIN_USER_PICTURE_FIELD, t('!field field is not exist in User entity.', array('!field' => $form_state['values'][TWO_STEPS_LOGIN_USER_PICTURE_FIELD])));
    }
  }
  if (!$form_state['values'][TWO_STEPS_LOGIN_USE_DEFAULT_USER_PICTURE] && !$form_state['values'][TWO_STEPS_LOGIN_USER_PICTURE_FIELD]) {
    form_set_error(TWO_STEPS_LOGIN_USER_PICTURE_FIELD, t('User picture field is required.'));
  }
  if (!count(form_get_errors())) {
    global $user;
    if (isset($form_state['values'][TWO_STEPS_LOGIN_DEFAULT_IMAGE]) && !empty($form_state['values'][TWO_STEPS_LOGIN_DEFAULT_IMAGE])) {
      $fid = $form_state['values'][TWO_STEPS_LOGIN_DEFAULT_IMAGE];
      $file = file_load($fid);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'two_steps_login', 'two_steps_login', $user->uid);
    }
    drupal_register_shutdown_function('menu_rebuild');
  }
}

/**
 * Get all available image styles.
 *
 * @return array
 *   List of image styles.
 */
function two_steps_login_get_all_image_styles() {
  $styles = array();
  foreach (image_styles() as $style_name => $style) {
    $styles[$style_name] = $style['label'];
  }
  return $styles;
}
