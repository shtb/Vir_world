#ifndef THREAD_CONTROL_H
#define THREAD_CONTROL_H

#include "../../pub/STDHF.h"

class thread_control
{
public:
	std::string thread_name;
	int t_run_times = 0;
	bool t_load_sy = false;
	bool t_loaded_sy = false;
	bool t_start_sy = false;
	bool t_started_sy = false;
	bool t_save_sy = false;
	bool t_saved_sy = false;
	bool t_bye_sy = false;
    bool t_finished_sy = false;

    virtual int t_run();

    virtual int t_load();
    virtual int t_start();
    virtual int t_loop();
    virtual int t_save();
    virtual int t_finish();

	thread_control();
	virtual ~thread_control();

protected:
private:
};

#endif // THREAD_CONTROL_H
