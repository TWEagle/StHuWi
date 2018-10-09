<?php

/**
 * @file
 * Two steps user login callback file for the two_steps_login module.
 */

/**
 * Building two steps login form.
 *
 * @ingroup forms
 */
function two_steps_login_get_login_form(&$form, &$form_state) {
  global $user;

  // If we are already logged on, go to the user page instead.
  if ($user->uid) {
    drupal_goto('user/' . $user->uid);
  }

  $form['#attached']['js'][] = drupal_get_path('module', 'two_steps_login') . '/js/two_steps_login.js';
  two_steps_login_add_js_settings();
  $form['#attached']['css'][] = drupal_get_path('module', 'two_steps_login') . '/css/two_steps_login.css';
  // If $form_state['step'] is not set, we set it to 1.
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : 1;
  // Add a wrapper that will be used by the Form API to update form using Ajax.
  $form['#prefix'] = '<div id="two_steps_login_ajax_form" class="two-steps-login-ajax-form">';
  $form['#suffix'] = '</div>';

  // Depending on which step of the form we are on, we output different fields.
  switch ($form_state['step']) {
    case 1:
      $form['#attributes']['class'][] = 'step-1';
      $form['names_markup'] = array(
        '#markup' => two_steps_login_get_names_markup(),
      );
      $default_value = isset($form_state['values']['name']) ? $form_state['values']['name'] : (isset($form_state['storage']['name']) ? $form_state['storage']['name'] : '');
      // Display login form:
      $default_image = file_load(two_steps_login_get_defaut_image());
      $image_url = !empty($default_image) ? two_steps_login_get_user_picture_url($default_image->uri, two_steps_login_get_image_style()) : '';
      $markup = '<div class="two-steps-login-username-input-wrapper">';
      if (!empty($image_url)) {
        $markup .= '<img id="user-default-pic" src="' . $image_url . '">';
      }
      else {
        $markup .= '<div id="user-default-pic" class="two-steps-login-user-picture-letter"><span class="two-steps-login-new-user">?</span></div>';
      }
      $form['picture_placeholder'] = array(
        '#markup' => $markup,
      );
      $form['name'] = array(
        '#type' => 'textfield',
        '#title' => two_steps_login_get_username_field_title(),
        '#size' => 60,
        '#maxlength' => USERNAME_MAX_LENGTH,
        '#required' => TRUE,
        '#default_value' => $default_value,
      );
      $form['name']['#description'] = two_steps_login_get_username_field_description();
      break;

    case 2:
      $form['#attributes']['class'][] = 'step-2';
      if (two_steps_login_is_user_picture_enable()) {
        two_steps_login_add_picture_field_if_enabled($form, $form_state);
      }
      $form['pass'] = array(
        '#type' => 'password',
        '#title' => t('Password'),
        '#description' => t('Enter the password that accompanies your username.'),
        '#required' => TRUE,
        '#attributes' => array('placeholder' => 'Password'),
      );
      break;
  }

  // Create a container for our buttons.
  $form['buttons'] = array(
    '#type' => 'container',
  );
  // If we are on step 1, we do not want a back button.
  if ($form_state['step'] !== 1) {
    $form['buttons']['back'] = array(
      '#type' => 'submit',
      '#value' => two_steps_login_get_back_button_name(),
      '#limit_validation_errors' => array(),
      '#submit' => array('two_steps_login_ajax_back_submit'),
      '#ajax' => array(
        'wrapper' => 'two_steps_login_ajax_form',
        'callback' => 'two_steps_login_ajax_form_ajax_callback',
      ),
    );
  }
  // We only want a forward button if we are on first step.
  if ($form_state['step'] === 1) {
    $form['buttons']['forward'] = array(
      '#type' => 'submit',
      '#value' => two_steps_login_get_next_button_name(),
      '#suffix' => '</div>',
      '#ajax' => array(
        'wrapper' => 'two_steps_login_ajax_form',
        'callback' => 'two_steps_login_ajax_form_ajax_callback',
      ),
    );
    $form['#validate'] = array('two_steps_login_username_validate');
  }
  // We only want a submit button if we are on the last step of the form.
  else {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => two_steps_login_get_login_button_name(),
    );
    $form['#validate'] = user_login_default_validators();
    array_unshift($form['#validate'], 'two_steps_login_user_login_validate');
    $form['actions'] = array('#type' => 'actions');
    $form['#submit'] = array('user_login_submit');
  }
}

/**
 * Form builder; the two step user login form.
 *
 * @ingroup forms
 */
function two_steps_login_user_login($form, &$form_state) {
  two_steps_login_get_login_form($form, $form_state);
  return $form;
}

/**
 * Back submit ajax callback.
 */
function two_steps_login_ajax_back_submit($form, &$form_state) {
  $form_state['step']--;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate callback.
 */
function two_steps_login_user_login_validate($form, &$form_state) {
  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $form_state['values'] = array_merge($form_state['values'], $form_state['storage']);
  }
}

/**
 * Submit callback.
 */
function two_steps_login_user_login_submit($form, &$form_state) {
  $form_state['storage'] = $form_state['values'];
  if (isset($form_state['values']['forward']) && $form_state['values']['op'] == $form_state['values']['forward']) {
    $form_state['step']++;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback.
 */
function two_steps_login_ajax_form_ajax_callback($form, &$form_state) {
  two_steps_login_set_cookie($form_state);
  return $form;
}

/**
 * Form builder; the two step user login form.
 *
 * @ingroup forms
 */
function two_steps_login_block($form, &$form_state) {
  two_steps_login_get_login_form($form, $form_state);
  return $form;
}

/**
 * Validate callback.
 */
function two_steps_login_block_validate($form, &$form_state) {
  if (isset($form_state['values']['submit']) && $form_state['values']['op'] == $form_state['values']['submit']) {
    $form_state['values'] = array_merge($form_state['values'], $form_state['storage']);
  }
}

/**
 * Submit callback.
 */
function two_steps_login_block_submit($form, &$form_state) {
  $form_state['storage'] = $form_state['values'];
  if (isset($form_state['values']['forward']) && $form_state['values']['op'] == $form_state['values']['forward']) {
    $form_state['step']++;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate username on first step.
 *
 * Sets a form error if username has not been authenticated.
 */
function two_steps_login_username_validate($form, &$form_state) {
  if (two_steps_login_is_email_registration_enabled()) {
    email_registration_user_login_validate($form, $form_state);
  }
  if (two_steps_login_is_logintoboggon_enabled()) {
    logintoboggan_user_login_validate($form, $form_state);
  }
  if (!two_steps_login_get_uid_from_username($form_state['values']['name'])) {
    $query = isset($form_state['values']['name']) ? array('name' => $form_state['values']['name']) : array();
    form_set_error('name', t('Sorry, unrecognized username. <a href="@password">Have you forgotten your username?</a>', array('@password' => url('user/password', array('query' => $query)))));
    watchdog('user', 'Login attempt failed for %user.', array('%user' => $form_state['values']['name']));
  }
}

/**
 * Get user uid from username.
 *
 * @param string $name
 *   Username of user.
 *
 * @return int
 *   Uid of user.
 */
function two_steps_login_get_uid_from_username($name) {
  return db_query("SELECT uid FROM {users} WHERE name = :name AND status = 1", array(':name' => $name))->fetchField();
}

/**
 * Add user picture field to form if it is enabled.
 */
function two_steps_login_add_picture_field_if_enabled(&$form, $form_state) {
  $uid = two_steps_login_get_uid_from_username($form_state['values']['name']);
  $image_url = two_steps_login_get_user_picture($uid);
  if (!empty($image_url)) {
    $markup = '<div class="two-steps-login-user-picture-big"><img id="user-picture-login" src="' . $image_url . '"><span class="two-steps-login-user-markup">' . $form_state['values']['name'] . '</span></div>';
  }
  else {
    $markup = '<div class="two-steps-login-user-picture-letter-big"><span class="two-steps-login-user-picture-letter-span tsl-' . strtolower($form_state['values']['name'][0]) . '">' . strtoupper($form_state['values']['name'][0]) . '</span><span class="two-steps-login-user-markup">' . $form_state['values']['name'] . '</span></div>';

  }
  $form['user_picture'] = array(
    '#markup' => $markup,
  );
}
