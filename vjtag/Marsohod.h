#ifndef MARSOHOD_H
#define MARSOHOD_H

#include <WinSock2.h>

#define STP_PORT 9998
#define BUFFER_SIZE 2048


class CMarsohod
{
public:
	CMarsohod(void);
	~CMarsohod(void);
	int start_tcl_script(const wchar_t *name);
	int stp_connect(void);
	int stp_disconnect(void);
protected:
	SOCKET m_sock;
public:
	int shift_ir(int instance_index, int ir_value, int nocaptured);
protected:
	char* m_buffer;
	char* m_answer;
	int get_answer();
public:
	int run_test_idle();
	int send_buffer_get_data(int data_size);
	int shift_dr_hex(char *dr_value, int instance_index, int dr_length, int nocaptured);
	int shift_dr_bit(char *dr_value, int instance_index, int nocaptured);
};

#endif
