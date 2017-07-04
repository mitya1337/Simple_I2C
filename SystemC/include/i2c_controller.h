#ifndef __I2C_CONTROLLER_H
#define __I2C_CONTROLLER_H

#include <systemc.h>

SC_MODULE(i2c_controller)
{
	//External interface
	sc_in<bool> clk;
	sc_in<bool> rst;
	sc_in<sc_uint<7>> addr;
	sc_in<sc_uint<8>> data_in;
	sc_in<bool> enable;
	sc_in<bool> rw;
	
	sc_out<sc_lv<8>> data_out;
	sc_out<bool> ready;
	
	sc_inout<sc_logic> i2c_sda;
	sc_inout<sc_logic> i2c_scl;
	
	enum states {IDLE, START, ADDRESS, READ_ACK, WRITE_DATA, WRITE_ACK, READ_DATA, READ_ACK2, STOP};
	sc_signal<states> state;
	
	sc_signal<bool> write_enable;
	sc_signal<sc_logic> sda_out;
	bool  i2c_scl_enable;
	sc_signal<bool> i2c_clk;
	sc_uint<8> saved_addr;
	sc_lv<8> saved_data;
	sc_uint<8> counter;
	sc_uint<8> counter2;
	sc_lv<8> data_from_slave;
	
	SC_CTOR(i2c_controller)
	{
		SC_METHOD(clk_divider);
		sensitive << clk.pos();
		SC_METHOD(scl_controller);
		sensitive << i2c_clk.negedge_event() << rst.pos();
		
		SC_METHOD(logic_fsm);
		sensitive << i2c_clk.posedge_event() << rst.pos();
		
		SC_METHOD(data_fsm);
		sensitive << i2c_clk.negedge_event() << rst.pos();
		
		SC_METHOD(ready_assign);
		sensitive << rst << state;
		
		SC_METHOD(i2c_scl_assign);
		sensitive  << i2c_clk;
		
	}
	
	void clk_divider();
	void scl_controller();
	void logic_fsm();
	void data_fsm();
	void ready_assign();
	void i2c_scl_assign();
};
#endif
