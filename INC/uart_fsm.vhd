-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): xbatyk00
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
    CLK      : in std_logic;
    RST      : in std_logic;
    DIN      : in std_logic;
    CLK_CNT  : in std_logic_vector(4 downto 0);
    BIT_CNT  : in std_logic_vector(3 downto 0);
    RX_EN    : out std_logic;
    CNT_EN   : out std_logic;
    DATA_VLD : out std_logic
);
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type FSM_STATE is (IDLE_STATE, START_BIT, RECEIVE_DATA, STOP_BIT);
signal state : FSM_STATE := IDLE_STATE;
begin

    RX_EN <= '1' when state = RECEIVE_DATA else '0';
    CNT_EN <= '1' when state = START_BIT or state = RECEIVE_DATA else '0';
    DATA_VLD <= '1' when state = STOP_BIT else '0';
    
    process (CLK) begin
        if rising_edge(CLK) then
            if RST = '1' then
                state <= IDLE_STATE;
            else
                case state is
                    when IDLE_STATE =>
                        if DIN = '0' then
                            state <= START_BIT;
                        end if;
                    when START_BIT =>
                        if CLK_CNT = "10110" then
                            state <= RECEIVE_DATA;
                        end if;
                    when RECEIVE_DATA =>
                        if BIT_CNT = "1000" then
                            state <= STOP_BIT;
                        end if;
                    when STOP_BIT =>
                        if DIN = '1' then
                            state <= IDLE_STATE;
                        end if;
                    when others => null;
                end case;
            end if;
        end if;
    end process;
end behavioral;
