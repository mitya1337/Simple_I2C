#include <systemc.h>

#include <i2c_controller.h>

void i2c_controller::clk_divider()
{
	if(rst)
	{
	  i2c_clk.write(1);
	}
	else
	{
		if(counter2 == 1)
		{	
			bool t = i2c_clk.read();
			i2c_clk.write(!t);
			counter2 = 0;		
		}
		 else {
			counter2 = counter2 + 1;
		}
	}
}

void i2c_controller::scl_controller()
{
	if(rst) 
	{
		i2c_scl_enable = 0;
	} 
	else 
	{
		if((state == IDLE) | (state == START) | (state == STOP)) 
		{
			i2c_scl_enable = 0;
		}
		else
		{
			i2c_scl_enable = 1;
		}
	} 
}

void i2c_controller::logic_fsm()
{
	if(rst) 
	{
		state = IDLE;
	}
	else
	{
		switch(state)
		{
			case IDLE:
			 if(enable == 1)
			 {
				 state = START;
				 saved_addr = (addr,rw);
				 saved_data = data_in;
			 }
			 else
			 {
				 state = IDLE;
			 }
			break;
			
			case START:
				counter = 7;
				state = ADDRESS;
			break;
			
			case ADDRESS:
				if(counter == 0)
				{
					state = READ_ACK;
				}
				else
				{
					counter = counter - 1;
				}
			break;
			
			case READ_ACK:
				if(i2c_sda ==(sc_logic) 0)
				{
					counter = 7;
					if(saved_addr[0] == 0) 
					{
						state = WRITE_DATA;
					}
					else
					{
						state = READ_DATA;
					}
				}
				else
				{
					state = STOP;
				}
			break;
			
			case WRITE_DATA:
				if(counter == 0)
				{
					state = READ_ACK2;
				}
				else
				{
					counter = counter - 1;
				}
			break;
			
			case READ_ACK2:
				if((i2c_sda == 0) & (enable == 1))
				{
					state = IDLE;
				}
				else
				{
					state = STOP;
				}
			break;
			
			case READ_DATA:
				data_from_slave[counter] = i2c_sda;
				if(counter == 0)
				{
					state = WRITE_ACK;
				}
				else
				{
					counter = counter - 1;
				}
			break;
			
			case WRITE_ACK:
				state = STOP;
			break;
			
			case STOP:
				state = IDLE;
			break;
		}
	}
}

void i2c_controller::data_fsm()
{
	if(rst)
	{
		write_enable.write(1);
		i2c_sda.write((sc_logic)1);
	}
	else
	{
		switch(state)
		{
			case IDLE:
			break;
			case READ_ACK2:
			break;
			case START:
				write_enable.write(1);
				i2c_sda.write((sc_logic)0);
			break;
			
			case ADDRESS:
				{
				int t = saved_addr[counter];
				i2c_sda.write((sc_logic) t);
				}
			break;
			
			case READ_ACK:
				i2c_sda.write(sc_logic('Z'));
				write_enable.write(0);
			break;
			
			case WRITE_DATA:
				{
				write_enable.write(1);
				sc_logic t = saved_data[counter];
				i2c_sda.write((sc_logic) t);
				}
			break;
			
			case WRITE_ACK:
				i2c_sda.write((sc_logic) 0);
				write_enable.write(1);
			break;
			
			case READ_DATA:
				write_enable.write(0);
			break;
			
			case STOP:
				write_enable.write(1);
				i2c_sda.write((sc_logic)1);
			break;
		}
	}
}

void i2c_controller::ready_assign()
{
	if((rst == 0) && (state == IDLE))
	{
		ready.write(1);
	}
	else
	{
		ready.write(0);
	}
}

void i2c_controller::i2c_scl_assign()
{
	if(i2c_scl_enable  == 0)
	{
		i2c_scl.write((sc_logic)1);
	}
	else
	{
		i2c_scl.write((sc_logic)i2c_clk);
	}
}
