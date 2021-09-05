-- uart.vhd: UART controller - receiving part
-- Author(s): xbatyk00
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-------------------------------------------------
entity UART_RX is
port(	
    CLK       : in std_logic;
    RST       : in std_logic;
    DIN       : in std_logic;
    DOUT      : out std_logic_vector(7 downto 0);
    DOUT_VLD  : out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
  
    signal clk_cnt  : std_logic_vector(4 downto 0);
    signal bit_cnt 	: std_logic_vector(3 downto 0);
    signal rx_en  	: std_logic;
    signal cnt_en 	: std_logic;
    signal data_vld : std_logic;
    signal clk_st	: std_logic := '0';
    
begin

    FSM: entity work.UART_FSM(behavioral)
    port map (
        CLK 	    => CLK,
        RST 	    => RST,
        DIN 	    => DIN,
        CLK_CNT	   	=> clk_cnt,
        BIT_CNT		=> bit_cnt,
        RX_EN     	=> rx_en,
        CNT_EN		=> cnt_en,
        DATA_VLD	=> data_vld
    );
    
    process (CLK) begin

    	DOUT_VLD <= data_vld;
    	
    	if clk_st = '0' then
			DOUT(0) <= '0';
			DOUT(1) <= '0';
			DOUT(2) <= '0';
			DOUT(3) <= '0';
			DOUT(4) <= '0';
			DOUT(5) <= '0';
			DOUT(6) <= '0';
			DOUT(7) <= '0';
			clk_st <= '1';
		end if;
		
        if rising_edge(CLK) then
        	
        	DOUT_VLD <= data_vld;
        	
            if cnt_en = '1' then
                clk_cnt <= clk_cnt + 1;
            else
                clk_cnt <= "00000";
            end if;
            
            if rx_en = '1' then
                if clk_cnt(4) = '1' then
                    clk_cnt <= "00000";
                    DOUT(to_integer(unsigned(bit_cnt))) <= DIN;
                    bit_cnt <= bit_cnt + 1;
                end if;
            else
            	bit_cnt <= "0000";
            end if;
            
        end if;
    end process; 
end behavioral;