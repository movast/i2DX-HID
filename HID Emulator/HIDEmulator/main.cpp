// 
// main.cpp
// Created by ilian on 2015-02-07
// Licenced under the Apache License, Version 2.0
//

#define OSCPKT_OSTREAM_OUTPUT
#include <interception.h>
#include "utils.h"
#include <iostream>
#include "oscpkt.hh"
#include "udp.hh"
#include <chrono>
#include <thread>

using std::cout;
using std::cerr;
using std::string;

using namespace oscpkt;

const int PORT_NUM = 57120;

enum ScanCode
{
	SCANCODE_Z = 0x2c,
	SCANCODE_S = 0x1f,
	SCANCODE_X = 0x2D,
	SCANCODE_D = 0x20,
	SCANCODE_C = 0x2e,
	SCANCODE_F = 0x21,
	SCANCODE_V = 0x2f,
	SCANCODE_BRACKET_OPEN = 0x1a,
	SCANCODE_QUOTATIONMARK = 0x28
};

// Thread that passes through keyboard input
void interceptThread(InterceptionContext* context) {
	InterceptionDevice device;
	InterceptionKeyStroke stroke;
	while (interception_receive(*context, device = interception_wait(*context), (InterceptionStroke *)&stroke, 1) > 0) {
		interception_send(*context, device, (InterceptionStroke *)&stroke, 1);
		cout << std::showbase << "Keyboard button " << std::hex << stroke.code << " pressed." << std::endl;
	}
}

int main()
{
	UdpSocket sock;
	sock.bindTo(PORT_NUM);

	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke stroke;
	raise_process_priority();
	context = interception_create_context();

	cout << "Please press any key on your keyboard to be selected for emulation." << std::endl;
	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
	device = interception_wait(context);
	std::thread first(interceptThread, (InterceptionContext *)&context );
	if (!sock.isOk()) {
		cerr  << "Error opening port " << PORT_NUM << ": " << sock.errorMessage() << "\n";
	}
	else {
		cout << std::dec << "Server started, will listen to packets on port " << PORT_NUM << std::endl;
		PacketReader pr;
		PacketWriter pw;
		while (sock.isOk()) {
			if (sock.receiveNextPacket(30 /* timeout, in ms */)) {
				pr.init(sock.packetData(), sock.packetSize());
				oscpkt::Message *msg;
				while (pr.isOk() && (msg = pr.popMessage()) != 0) {
					int iarg;
					cout << "Server: received message " << msg->addressPattern() <<" from " << sock.packetOrigin() << "\n";
					if (msg->match("/iidx_1").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_Z;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/iidx_2").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_S;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/iidx_3").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_X;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/iidx_4").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_D;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/iidx_5").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_C;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/iidx_6").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_F;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/iidx_7").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_V;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/turntable_cw").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_BRACKET_OPEN;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else if (msg->match("/turntable_ccw").popInt32(iarg).isOkNoMoreArgs()) {
						stroke.code = SCANCODE_QUOTATIONMARK;
						stroke.state = iarg == 1 ? 0x00 : 0x01;
					}
					else {
						cout << "Server: unhandled message: " << *msg << "\n";
					}
					interception_send(context, device, (InterceptionStroke *)&stroke, 1);
				}
			}
		}
	}
	interception_destroy_context(context);
	return 0;
}
