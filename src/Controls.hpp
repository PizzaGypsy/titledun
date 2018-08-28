#pragma once

/*
  This class is for setting up a keymap by manipulating the key_pressed and mouse_pressed arrays in MainApp.
*/

#include "MainApp.hpp"
#include "GraphicalInterface.hpp"

extern float beta;

enum CAMERA_KEY{C_MOUSE1 = 0,
				C_MOUSE2 = 1,
				C_MOUSE3 = 2};

enum KEY_MAP{
  BACKSPACE_KEY = 8,
  TAB_KEY = 9,
  ENTER_KEY = 13,
  SHIFT_KEY = 16,
  CTRL_KEY = 17,
  ALT_KEY = 18,
  PAUSE_KEY = 19,
  CAPS_KEY = 20,
  ESC_KEY = 27,
  PAGE_UP_KEY = 33,
  PAGE_DOWN_KEY = 34,
  END_KEY = 35,
  HOME_KEY = 36,
  LEFT_ARROW_KEY = 37,
  UP_ARROW_KEY = 38,
  RIGHT_ARROW_KEY = 39,
  DOWN_ARROW_KEY = 40,
  INSERT_KEY = 45,
  DELETE_KEY = 46,
  ZERO_KEY = 48,
  ONE_KEY = 49,
  TWO_KEY = 50,
  THREE_KEY = 51,
  FOUR_KEY = 52,
  FIVE_KEY = 53,
  SIX_KEY = 54,
  SEVEN_KEY = 55,
  EIGHT_KEY = 56,
  NINE_KEY = 57,
  A_KEY = 65,
  B_KEY = 66,
  C_KEY = 67,
  D_KEY = 68,
  E_KEY = 69,
  F_KEY = 70,
  G_KEY = 71,
  H_KEY = 72,
  I_KEY = 73,
  J_KEY = 74,
  K_KEY = 75,
  L_KEY = 76,
  M_KEY = 77,
  N_KEY = 78,
  O_KEY = 79,
  P_KEY = 80,
  Q_KEY = 81,
  R_KEY = 82,
  S_KEY = 83,
  T_KEY = 84,
  U_KEY = 85,
  V_KEY = 86,
  W_KEY = 87,
  X_KEY = 88,
  Y_KEY = 89,
  Z_KEY = 90,
  LEFT_WIN_KEY = 91,
  RIGHT_WIN_KEY = 92,
  SELECT_KEY = 93,
  NUM_0 = 96,
  NUM_1 = 97,
  NUM_2 = 98,
  NUM_3 = 99,
  NUM_4 = 100,
  NUM_5 = 101,
  NUM_6 = 102,
  NUM_7 = 103,
  NUM_8 = 104,
  NUM_9 = 105,
  MULT_KEY = 106,
  ADD_KEY = 107,
  SUB_KEY = 109,
  DEC_KEY = 110,
  DIV_KEY = 111,
  F1_KEY = 112,
  F2_KEY = 113,
  F3_KEY = 114,
  F4_KEY = 115,
  F5_KEY = 116,
  F6_KEY = 117,
  F7_KEY = 118,
  F8_KEY = 119,
  F9_KEY = 120,
  F10_KEY = 121,
  F11_KEY = 122,
  F12_KEY = 123,
  NUM_LOCK_KEY = 144,
  SCROLL_LOCK_KEY = 145,
  SEMI_COL_KEY = 186,
  EQUAL_KEY = 187,
  COMMA_KEY = 188,
  DASH_KEY = 189,
  PERIOD_KEY = 190,
  FOR_SLASH_KEY = 191,
  TILDE_KEY = 192,
  OPEN_BRACK_KEY = 219,
  BAK_SLASH_KEY = 220,
  CLOSE_BRACK_KEY = 221,
  SINGLE_QUOTE = 222,
};

class Controls {
public:

  static float alpha;
  
  static void mouse_event_handler(const Event* e, void* data) {
	if (e->get_name() == "mouse1")
	  M_A->mouse_pressed[C_MOUSE1] = true;
	else if (e->get_name() == "mouse1-up")
	  M_A->mouse_pressed[C_MOUSE1] = false;
  
	else if (e->get_name() == "mouse2")
	  M_A->mouse_pressed[C_MOUSE2] = true;
	else if (e->get_name() == "mouse2-up")
	  M_A->mouse_pressed[C_MOUSE2] = false;

	else if (e->get_name() == "mouse3")
	  M_A->mouse_pressed[C_MOUSE3] = true;
	else if (e->get_name() == "mouse3-up")
	  M_A->mouse_pressed[C_MOUSE3] = false;
  }

  static void key_event_handler(const Event* e, void* data) {
	std::cout << "event: " << e->get_name() << std::endl;
	if (e->get_name() == "escape")
	  M_A->key_pressed[ESC_KEY] = true;
	else if (e->get_name() == "escape-up")
	  M_A->key_pressed[ESC_KEY] = false;

	else if (e->get_name() == "`")
	  M_A->key_pressed[TILDE_KEY] = true;
	else if (e->get_name() == "`-up")
	  M_A->key_pressed[TILDE_KEY] = false;

	else if (e->get_name() == "e")
	  M_A->key_pressed[E_KEY] = true;
	else if (e->get_name() == "e-up")
	  M_A->key_pressed[E_KEY] = false;

	else if (e->get_name() == "d")
	  M_A->key_pressed[D_KEY] = true;
	else if (e->get_name() == "d-up")
	  M_A->key_pressed[D_KEY] = false;

	else if (e->get_name() == "s")
	  M_A->key_pressed[S_KEY] = true;
	else if (e->get_name() == "s-up")
	  M_A->key_pressed[S_KEY] = false;

	else if (e->get_name() == "f")
	  M_A->key_pressed[F_KEY] = true;
	else if (e->get_name() == "f-up")
	  M_A->key_pressed[F_KEY] = false;
  }
  
  static void define_keys();

  AsyncTask::DoneStatus move_player(GenericAsyncTask* task, void* data);
  
private:
  //bool mouse_pressed[3] = {false, false, false};
};
  
//extern CAMERA_KEY key;
