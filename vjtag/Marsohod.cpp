#include <process.h>
#include <stdio.h>

#include "Marsohod.h"
#include "MarsohodError.h"
void execute_script(void *param)
{
	wchar_t *name =(wchar_t*)param;
	_wsystem(name);
}


CMarsohod::CMarsohod(void) : m_sock (INVALID_SOCKET)
{
	m_buffer = new char[BUFFER_SIZE];
	m_answer = new char[BUFFER_SIZE];
}


CMarsohod::~CMarsohod(void)
{
	delete[] m_buffer;
	delete[] m_answer;
}


int CMarsohod::start_tcl_script(const wchar_t* name)
{
	_beginthread(execute_script, 0, (void*) name);
	return 0;
}


int CMarsohod::stp_connect(void)
{
	int iResult;
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		CMarsohodError::LogError(L"Connect to STP script failed.", __FILE__, __LINE__, WSAGetLastError());
		return 1;
	}
	
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(STP_PORT);

	iResult = connect(m_sock, (SOCKADDR *) &clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		CMarsohodError::LogError(L"Connect to STP script failed.", __FILE__, __LINE__, WSAGetLastError());
		iResult = closesocket(m_sock);
		m_sock = INVALID_SOCKET;
		if (iResult == SOCKET_ERROR)
			CMarsohodError::LogError(L"", __FILE__, __LINE__, WSAGetLastError());
		return 1;
	}

	LOG_MESSAGE(L"Connected to STP script.");

	return 0;
}


int CMarsohod::stp_disconnect(void)
{
	if (m_sock != INVALID_SOCKET) {
		int fact_buf_size = _snprintf(m_buffer, BUFFER_SIZE, "4\n");
		int result = send_buffer_get_data(fact_buf_size);
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
		LOG_MESSAGE(L"Disconnected from STP script.");
	}
	return 0;
}


int CMarsohod::shift_ir(int instance_index, int ir_value, int nocaptured)
{
	int fact_buf_size, result;
	if (INVALID_SOCKET == m_sock) return -1;
	fact_buf_size = _snprintf(m_buffer, BUFFER_SIZE, "1,%d,%d,%d\n", instance_index, ir_value, nocaptured);
	result = send_buffer_get_data(fact_buf_size);
	return result;
}


int CMarsohod::get_answer()
{
	int result;
	if (m_sock == INVALID_SOCKET) return -1;
	result = recv(m_sock, m_answer, BUFFER_SIZE, 0);
	if (result >= 0) {
		m_answer[result] = 0;
		for (int pos = 0; pos < result; ++pos) {
			if (m_answer[pos] == '\r' || m_answer[pos] == '\n') {
				m_answer[pos] = 0;
				result = pos;
			}
		}
	}
	else m_answer[0] = 0;
	return result;
}


int CMarsohod::run_test_idle()
{
	int fact_buf_size, result;
	if (m_sock == INVALID_SOCKET) return -1;
	fact_buf_size = _snprintf(m_buffer, BUFFER_SIZE, "0\n");
	result = send_buffer_get_data(fact_buf_size);
	return 0;
}


int CMarsohod::send_buffer_get_data(int data_size)
{
	int result;
	if (m_sock == INVALID_SOCKET) return -1;
	result = send(m_sock, m_buffer, data_size, 0);
	if (result != data_size) {
		return -1;
	}
	result = recv(m_sock, m_answer, BUFFER_SIZE, 0);
	if (result >= 0) {
		m_answer[result] = 0;
		for (int pos = 0; pos < result; ++pos) {
			if (m_answer[pos] == '\r' || m_answer[pos] == '\n') {
				m_answer[pos] = 0;
				result = pos;
			}
		}
	}
	else m_answer[0] = 0;
	return result;
}


int CMarsohod::shift_dr_hex(char *dr_value, int instance_index, int dr_length, int nocaptured)
{
	int fact_buf_size, result;
	if (INVALID_SOCKET == m_sock) return -1;
	fact_buf_size = _snprintf(m_buffer, BUFFER_SIZE, "2,%s,%d,%d,%d\n", dr_value, instance_index, dr_length, nocaptured);
	result = send_buffer_get_data(fact_buf_size);
	return result;
}


int CMarsohod::shift_dr_bit(char *dr_value, int instance_index, int nocaptured)
{
	int fact_buf_size, result;
	if (INVALID_SOCKET == m_sock) return -1;
	fact_buf_size = _snprintf(m_buffer, BUFFER_SIZE, "3,%s,%d,%d\n", dr_value, instance_index, nocaptured);
	result = send_buffer_get_data(fact_buf_size);
	return result;
}
