#include <systemc.h>

#include <i2c_slave_controller.h>

void i2c_slave_controller::start_condition()
{
	if((start == 0) && (scl == 1))
	{
		start = 1;
		counter = 7;
	}
}

void i2c_slave_controller::stop_condition()
{	
	if((start == 1) && (scl ==(sc_logic) 1)) 
	{	
		state = READ_ADDR;
		start = 0;
		write_enable = 0;
	}
}

void i2c_slave_controller::logic_fsm()
{
	if(start == 1)
	{
		switch(state)
		{
			case READ_ADDR:
				addr[counter] = sda.read();
				if(counter == 0)
				{
					state = SEND_ACK;
				}
				else
				{
					counter = counter - 1;
				}
			break;
			
			case SEND_ACK:
				if(addr.range(7,1) == ADDRESS)
				{
					counter = 7;
					if(addr[0] == 0)
					{
						state = READ_DATA;
					}
					else
					{
						state = WRITE_DATA;
					}
				}
			break;
			
			case READ_DATA:
				data_in[counter] = sda;
				if(counter == 0)
				{
					state = SEND_ACK2;
				}
				else
				{
					counter = counter - 1;
				}
			break;
			
			case SEND_ACK2:
				state = READ_ADDR;
			break;
			
			case WRITE_DATA:
				if(counter == 0)
				{
					state = READ_ADDR;
				}
				else
				{
					counter = counter - 1;
				}
			break;
		}
	}
}

void i2c_slave_controller::data_fsm()
{
	switch(state)
	{
		case READ_ADDR:
			write_enable.write(0);
		break;
		
		case SEND_ACK:
			if(addr.range(7,1) == ADDRESS)
			{
			sda.write((sc_logic) 0);
			write_enable.write(1);
			}
		break;
		
		case READ_DATA:
			write_enable.write(0);
		break;
		
		case WRITE_DATA:
			{
			int t = data_out[counter];
			sda.write((sc_logic) t);
			write_enable.write(1);
			}
		break;
		
		case SEND_ACK2:
			write_enable.write(1);
		break;
	}
}
