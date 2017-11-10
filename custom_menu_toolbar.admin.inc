<?php

/**
 * @file
 * Administrative pages.
 *
 * Created by: Topsitemakers
 * http://www.topsitemakers.com/
 */

/**
 * Main configuration form for administrators.
 */
function custom_menu_toolbar_admin_settings_form() {
  $form = array();
  $menus = menu_get_menus();
  $roles = user_roles($membersonly = TRUE, $permission = 'manage w3b4 toolbar');
  $results = db_query("SELECT * FROM {custom_menu_toolbar_role_menus} ORDER BY weight ASC");

  // Add the "no menu" option to the list.
  $menus = array_merge(array(CUSTOM_MENU_TOOLBAR_NO_MENU => t('- No toolbar -')), $menus);

  if (count($roles)) {
    $form['menus']['#tree'] = TRUE;
    foreach ($results as $result) {
      $role = user_role_load($result->rid);
      $form['menus'][$result->rid] = array(
        'name' => array(
          '#markup' => filter_xss_admin($role->name),
        ),
        'menu' => array(
          '#type' => 'select',
          '#options' => $menus,
          '#default_value' => $result->menu_name,
        ),
        'weight' => array(
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#default_value' => $result->weight,
          '#delta' => 10,
          '#title_display' => 'invisible',
        ),
      );
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Changes'),
    );
  }
  else {
    drupal_set_message(t('There are no user roles that have access to the W3b4 Toolbar module. Please <a href="@link">configure that</a> and then come back to this page to select which menus should be displayed for which role.', array(
      '@link' => url('admin/people/permissions', array('query' => array('destination' => current_path()))),
    )), 'notice');
  }

  return $form;
}

/**
 * Submit handler for the custom_menu_toolbar_admin_settings_form().
 */
function custom_menu_toolbar_admin_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['menus'] as $rid => $data) {
    db_update('custom_menu_toolbar_role_menus')
      ->condition('rid', $rid)
      ->fields(array(
        'menu_name' => $data['menu'],
        'weight' => $data['weight'],
      ))
      ->execute();
  }
  drupal_set_message(t('Changes have been saved successfully.'));
}
