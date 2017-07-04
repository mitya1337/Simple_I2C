#include <systemc.h>

#include <i2c_controller_tb.h>
#include <i2c_controller.h>
#include <i2c_slave_controller.h>

int sc_main(int argc, char* argv[])
{
    sc_signal<bool> rst;
    sc_signal<sc_uint<7>> addr;
    sc_signal<sc_uint<8>> data_in;
    sc_signal<bool> enable;
    sc_signal<bool> rw;
    sc_signal<sc_lv<8>> data_out;
    sc_signal<bool> ready;
    sc_signal<sc_logic, SC_MANY_WRITERS> i2c_sda;
    sc_signal<sc_logic> i2c_scl;


    sc_signal<sc_logic> scl;
    sc_signal<sc_logic> sda;
    sc_trace_file *f_trace;
    
    i2c_controller_tb tb("tb");
        
    f_trace = sc_create_vcd_trace_file("waveforms");
    f_trace->set_time_unit(1.0, SC_PS);
    
    sc_trace(f_trace, *tb.clk, "clk");
    sc_trace(f_trace, tb.rst, "rst");
    sc_trace(f_trace, tb.master->i2c_sda, "sda");
    sc_trace(f_trace, tb.ready, "ready");
    sc_trace(f_trace, tb.master->i2c_scl, "scl");
    sc_trace(f_trace, tb.addr, "addr");
    sc_trace(f_trace, tb.rw, "rw");
    sc_trace(f_trace, tb.enable, "enable");
    sc_trace(f_trace, tb.master->i2c_clk, "i2c_clk");
    sc_trace(f_trace, tb.master->i2c_scl_enable, "i2c_scl_enable");
    sc_trace(f_trace, tb.master->sda_out, "sda_out");
    sc_trace(f_trace, tb.master->counter, "counter");
    sc_trace(f_trace, tb.master->write_enable, "write_enable");
    sc_trace(f_trace, tb.master->saved_data, "saved_data");
    sc_trace(f_trace, tb.master->data_from_slave, "data_from_slave");
    sc_trace(f_trace, tb.slave->data_in, "slave_data_in");
    sc_trace(f_trace, tb.slave->data_out, "slave_data_out");
    sc_trace(f_trace, tb.slave->start, "slave_start");
    sc_trace(f_trace, tb.slave->write_enable, "slave_we");
    sc_trace(f_trace, tb.slave->addr, "slave_addr");
    sc_trace(f_trace, tb.slave->sda, "slave_sda");
    sc_trace(f_trace, tb.slave->scl, "slave_scl");
    sc_start(3000, SC_NS);
    sc_stop();

    sc_close_vcd_trace_file(f_trace);
    
    return 0;
}
