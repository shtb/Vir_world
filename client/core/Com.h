#ifndef COM
#define COM

#include "../../pub/STDHF.h"
#include <queue>
#include "input_method.h"
#include "../../pub/thread_control.h"
#include<semaphore.h>

using std::queue;
using std::stringstream;

enum input_receiver_enum { default_input, material_change_input, command_input, talk_input };

enum input_method_enum { sc_input = 1 };

enum Com_Keyboard { key_down, key_up };

enum Com_Keyon
{
      kF1 = 0,
      kF2,
      kup,
      kdown,
      kleft,
      kright,
      kpgup,
      kpgdown,
      khome,
      kend,
      klshift,
      krshift,
      klctrl,
      krctrl,
      klalt,
      kralt
};

struct key_board_node
{
      int key;
      int state;
};

class Com : public thread_control
{
public:
      char cub_com=0;
      char cub_state=0;
      stringstream cub_ss;
      bool input_lock=true;
      input_method input_sc;
      bool keys_on[16];
      int keys_on_last[16];
      queue<key_board_node> key_board;
      sem_t com_sem;
      int input_receiver = 0;
      int using_method = 0;
      Com();
      bool global_input_receiver(key_board_node key);
      void command_input_receiver(key_board_node key);
      void talk_input_receiver(key_board_node key);
      bool input_method_sc_receiver(key_board_node key);
      void default_input_receiver(key_board_node key);
      void material_change_receiver(key_board_node key);

      void push_key(key_board_node& key)
      {
            key_board.push(key);
            sem_post(&com_sem);
      }

      int t_load();
      int t_loop();
      int t_save();

      ~Com();
private:
};

#endif // COM
