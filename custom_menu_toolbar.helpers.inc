<?php

/**
 * @file
 * Helper functions provided by the Custom Menu Toolbar module.
 *
 * Created by: Topsitemakers
 * http://www.topsitemakers.com/
 */

/**
 * Helper function - get toolbar menu for current user.
 */
function _custom_menu_toolbar_get_menu_for_current_user() {
  // Sanity check - stop if the user does not have sufficient permissions.
  if (!user_access(CUSTOM_MENU_TOOLBAR_ROLE_ACCESS_TOOLBAR)) return NULL;

  global $user;
  $user_roles = array_keys($user->roles);

  return db_select('custom_menu_toolbar_role_menus', 'm')
    ->fields('m')
    ->condition('rid', $user_roles, 'IN')
    ->orderBy('weight', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchObject();
}
