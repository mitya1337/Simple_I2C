#ifndef __I2C_CONTROLLER_TB_H
#define __I2C_CONTROLLER_TB_H

#include <systemc.h>

#include <i2c_controller.h>
#include <i2c_slave_controller.h>

SC_MODULE(i2c_controller_tb)
{
	
	sc_clock *clk;
	sc_signal<bool> rst;
	sc_signal<sc_uint<7>> addr;
	sc_signal<sc_uint<8>> data_in;
	sc_signal<bool> enable;
	sc_signal<bool> rw;
	sc_signal<sc_lv<8>> data_out;
	sc_signal<bool> ready;
	sc_signal<sc_logic, SC_MANY_WRITERS> i2c_sda;
	sc_signal<sc_logic> i2c_scl;
	
	i2c_controller *master;
	i2c_slave_controller *slave;
	
	SC_CTOR(i2c_controller_tb)
	{
		clk = new sc_clock("clk",2,SC_NS);
		
		master = new i2c_controller("i2c_controller");
			master->clk(*clk);
			master->rst(rst);
			master->addr(addr);
			master->data_in(data_in);
			master->enable(enable);
			master->rw(rw);
			master->data_out(data_out);
			master->ready(ready);
			master->i2c_sda(i2c_sda);
			master->i2c_scl(i2c_scl);
			
		slave = new i2c_slave_controller("i2c_slave_controller");
			slave->sda(i2c_sda);
			slave->scl(i2c_scl);
			
		SC_THREAD(stimuli);
	 	sensitive << *clk << rst;

	}
	
	~i2c_controller_tb()
	{
		delete master;
		delete slave;
	}
	
	void stimuli();
	
};
#endif
