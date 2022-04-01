-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru

   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

--Programovy citac ROM (PC)
signal pc_data          : std_logic_vector(11 downto 0);
signal PC_INC, PC_DEC   : std_logic;

--Programovy citac RAM (PTR)
signal ptr_data         : std_logic_vector(9 downto 0);
signal PTR_INC, PTR_DEC : std_logic;

--Counter
signal cnt_data         : std_logic_vector(7 downto 0);
signal CNT_INC, CNT_DEC : std_logic;

--Automat (FSM)
type fsm is (
  IDLE,
  START,
  DECODE,
  POINT_INC,
  POINT_DEC,
  DATA_INC,DATA_INC2,
  DATA_DEC,DATA_DEC2,
  PUTCHAR,
  GETCHAR,
  NUL,
  NOPE,
  WHILE1,WHILE2,WHILE3,WHILE4,
  WHILE_BREAK1,WHILE_BREAK2,WHILE_BREAK3,
  WHILE_END1,WHILE_END2,WHILE_END3,WHILE_END4,WHILE_END5);
  
signal this_state, next_state : fsm;

signal sel: std_logic_vector(1 downto 0);


begin
  --Programovy citac ROM (PC)
  PC: process (RESET, CLK)
  begin
    if (RESET = '1') then pc_data <= (others => '0');
    elsif rising_edge(CLK) then
      if (PC_INC = '1') then  pc_data <= pc_data + 1;
      elsif (PC_DEC = '1') then pc_data <= pc_data - 1;
      end if;
    end if;
  end process PC;
  --Programovy citac RAM (PTR)
  PTR: process (RESET, CLK)
  begin
    if (RESET = '1') then ptr_data <= (others => '0');
    elsif rising_edge(CLK) then
      if (PTR_INC = '1') then ptr_data <= ptr_data + 1;
      elsif (PTR_DEC = '1') then ptr_data <= ptr_data - 1;
      end if;
    end if;
  end process PTR;
  --Counter
  CNT: process (RESET, CLK)
  begin
    if (RESET = '1') then cnt_data <= (others => '0');
    elsif rising_edge(CLK) then
      if (CNT_INC = '1') then cnt_data <= cnt_data + 1;
      elsif (CNT_DEC = '1') then cnt_data <= cnt_data - 1;
      end if;
    end if;
  end process CNT;

   MUX: process(DATA_RDATA, IN_DATA, sel, CLK)
 	 begin
  		if    (sel = "00") then DATA_WDATA <= IN_DATA;
		elsif (sel = "01")	then DATA_WDATA <= DATA_RDATA + 1;
		elsif (sel = "10")	then DATA_WDATA <= DATA_RDATA - 1; 
		elsif (sel = "11") then DATA_WDATA <= X"00";
		end if;
 	 end process MUX;
   	
   	DATA_ADDR <= ptr_data;
    CODE_ADDR <= pc_data;
	
--Cast konecneho automatu, ktera prepina stavy automatu
 fsm_state: process(CLK, RESET,EN)
 begin
    if (RESET = '1') then this_state <= IDLE;
    elsif rising_edge(CLK) and (EN = '1') then this_state <= next_state;
    end if;
 end process fsm_state;
 
 
--Konecny automat, ktery ridi cely procesor
 fsm_main_process: process(CLK, RESET, EN, CODE_DATA, IN_VLD, IN_DATA, DATA_RDATA, OUT_BUSY, this_state, cnt_data, sel)
 begin

    CODE_EN		 <= '0';
    DATA_EN		 <= '0';
    DATA_WREN	<= '0';
    IN_REQ	  	<= '0';
    OUT_WREN		<= '0';
    
    PC_INC		  <= '0';
    PC_DEC		  <= '0';
    PTR_INC		 <= '0';
    PTR_DEC		 <= '0';
    CNT_INC		 <= '0';
    CNT_DEC	  <= '0';
    sel	      <= "00";
    
    next_state <= IDLE;
    
    case this_state is

        when IDLE => next_state <= START;

        when START => 
					CODE_EN <= '1';
          next_state <= DECODE;
                      
        when NUL  => next_state <= NUL;                --jestli dostaneme null tak ukoncime

        when NOPE => PC_INC     <= '1';                --jestli dostaneme neznamy znak ignorujeme a jdeme dal
                     next_state <= START;

        when DECODE => case CODE_DATA is
                           when X"3E"	 => next_state  <= POINT_INC;    -- >
                           when X"3C"	 => next_state  <= POINT_DEC;	   -- <
                           when X"2B"	 => next_state  <= DATA_INC;	    -- +
                           when X"2D"	 => next_state  <= DATA_DEC;	    -- -
                           when X"2E"	 => next_state  <= PUTCHAR;	     -- .
                           when X"2C"	 => next_state  <= GETCHAR;	     -- ,
                           when X"00"	 => next_state  <= NUL;		        -- null   
                           when X"7E"	 => next_state  <= WHILE_BREAK1;	-- ~
                           when X"5B"	 => next_state  <= WHILE1;	      -- [
                           when X"5D"	 => next_state  <= WHILE_END1;	  -- ]
                           when others => next_state  <= NOPE;         -- vsichni ostatni znaky
                        end case;
								
        --Incrementujeme ukazatel na bunku (>)
        when POINT_INC => PC_INC     <= '1';           --dasli prikaz
                          PTR_INC    <= '1';           --prepneme se na dalsi bunku
                          next_state <= START;         --prepneme na stav START pro nacitani dalsich instrukci


        --Dekrementujeme ukazatel na bunku (<)
        when POINT_DEC => PC_INC     <= '1';           --dalsi prikaz
                          PTR_DEC    <= '1';           --prepneme se na minulu bunku
                          next_state <= START;         --prepneme na stav START pro nacitani dalsich instrukci


        --Incrementujeme hodnotu bunky (+)
        when DATA_INC  => DATA_EN    <= '1';           --povolime pracovat s datami
                          DATA_WREN  <= '0';           --povolime nacitani
                          next_state <= DATA_INC2;
        when DATA_INC2 => DATA_EN    <= '1';           --povolime pracovat s datami
                          DATA_WREN  <= '1';           --povolime zapisovani
                          PC_INC     <= '1';           --dalsi prikaz
                          sel        <= "01";          --zapiseme do RAM hodnotu bunky o jednicku vic
                          next_state <= START;         --prepneme na stav START pro nacitani dalsich instrukci


        --Decrementujeme hodnotu bunky (-)
        when DATA_DEC  => DATA_EN    <= '1';           --povolime pracovat s datami
                          DATA_WREN  <= '0';           --povolime nacitani
                          next_state <= DATA_DEC2;
        when DATA_DEC2 => DATA_EN    <= '1';           --povolime pracovat s datami
                          DATA_WREN  <= '1';           --povolime zapisovani
                          PC_INC     <= '1';           --dalsi prikaz
                          sel        <= "10";          --zapiseme do RAM hodnotu bunky o jednicku mene
                          next_state <= START;         --prepneme na stav START pro nacitani dalsich instrukci
                          
                          
        --Odesilame hodnotu na vystup                  
        when PUTCHAR => if(OUT_BUSY = '0') then
                            PC_INC		   <= '1';         --dalsi prikaz
                            DATA_EN    <= '1';         --povolime pracovat s datami
                            DATA_WREN	 <= '0';         --povolime nacitani
                            OUT_WREN		 <= '1';         --povolime zapis
                            OUT_DATA	  <= DATA_RDATA;  --ASCII hodnota
                            next_state	<= START;       --prepneme na stav START pro nacitani dalsich instrukci
                          else
                            next_state	<= PUTCHAR;     --cekame pokud OUT_BUSY nebude 0      
                          end if;
                          
       --Nacitame hodnotu v bunku
       when GETCHAR =>    IN_REQ		   <= '1';		         --nastavime aby mohl procesor nacitat daty z klavesnici
                       if (IN_VLD = '1') then          --cekame dokud data bude validna
                          DATA_EN		  <= '1';           --povolime pracovat s datami
                          DATA_WREN	 <= '1';           --povolime zapisovani
                          PC_INC		   <= '1';           --dalsi prikaz
                          IN_REQ		   <= '0';           --nacli musime vypnout
                          sel       	<= "00";          --zapiseme do RAM hodnotu bunky
                          next_state	<= START;         --prepneme na stav START pro nacitani dalsich instrukci
                       end if;
                       next_state <= GETCHAR;
       --WHILE START                 
       when WHILE1 => PC_INC		   <= '1';               --dalsi prikaz
				              DATA_EN		  <= '1';               --povolime pracovat s datami
				              DATA_WREN	 <= '0';               --povolime nacitani
				              next_state	<= WHILE2;
		
			 when WHILE2 => if (DATA_RDATA = 0) then next_state <= WHILE3;          --pokud je nulta tak pokracujeme
				              else next_state <= START;                               --prepneme na stav START pro nacitani dalsich instrukci
				              end if;
				              
			 when WHILE3 => if (cnt_data /= "00000000") then  
					             CODE_EN		<= '1';                                        --povlujeme aktualizce CODE_DATA
					             next_state	<= WHILE4;                                   --prepneme na stav START pro nacitani dalsich instrukci                                   
				              else
					             CNT_INC		<= '1';                                        --incrementujeme CNT
					             next_state	<= START; 
				              end if;
				              
			 when WHILE4 => if    (CODE_DATA = X"5B")  then	CNT_INC	<= '1';          --jestli dostali ']' tak decremenrujeme CNT,
				              elsif (CODE_DATA = X"5D")  then CNT_DEC	<= '1';          --jestli '[' tak incrementujeme
				              end if;         
				              PC_INC		   <= '1';                                       --dalsi prikaz
				              next_state	<= WHILE3;

			--WHILE BREAK
			when WHILE_BREAK1 => PC_INC		   <= '1';                                  --dalsi prikaz
				                   CNT_INC		  <= '1';                                  --incrementujeme CNT
				                   next_state	<= WHILE_BREAK2;
				                   
			when WHILE_BREAK2 => if (cnt_data /= "00000000") then
					                   CODE_EN		  <= '1';                                --povlujeme aktualizce CODE_DATA
					                   next_state	<= WHILE_BREAK3;                             
				                   else
					                   next_state	<= START;                              --prepneme na stav START pro nacitani dalsich instrukci
				                   end if;
				                   
			when WHILE_BREAK3 => if    (CODE_DATA = X"5B")  then	CNT_INC	<= '1';     --jestli dostali ']' tak decremenrujeme CNT,
				                   elsif (CODE_DATA = X"5D")  then CNT_DEC	<= '1';     --jestli '[' tak incrementujeme
				                   end if;
				                   PC_INC	<= '1';                                      --dalsi prikaz
				                   next_state	<= WHILE_BREAK2;

			--WHILE END
			when WHILE_END1 => DATA_EN		  <= '1';                                     --povolime pracovat s datami
			                   DATA_WREN	 <= '0';                                     --povolime nacitani
				                 next_state	<= WHILE_END2;
		
			when WHILE_END2 => if (DATA_RDATA = 0) then                    
					                PC_INC		   <= '1';                                    --dalsi prikaz
					                next_state	<= START;                                  --prepneme na stav START pro nacitani dalsich instrukci
				                 else
					                CNT_INC		  <= '1';                                    --incrementujeme CNT
					                PC_DEC		   <= '1';                                    --decrementujeme citac
					                next_state	<= WHILE_END3;                   
				                 end if;
			when WHILE_END3 => if (cnt_data = 0) then
					                      next_state	<= START;                            --prepneme na stav START pro nacitani dalsich instrukci
				                       else
					                      CODE_EN		<= '1';                                --povlujeme aktualizce CODE_DATA
					                      next_state	<= WHILE_END4;                       
				                       end if;
				                       
			when WHILE_END4 =>if    (CODE_DATA = X"5B")  then	CNT_DEC	<= '1';         --jestli dostali '[' tak decremenrujeme CNT,
				                elsif (CODE_DATA = X"5D")  then CNT_INC	<= '1';         --jestli ']' tak incrementujeme                      
				                end if;
				                next_state	<= WHILE_END5;
			when WHILE_END5 =>if (cnt_data = 0) then PC_INC	<= '1';                   --jestli cnt_data neni nulta decrementujeme citac a pokracujeme
				                else PC_DEC <= '1';                                     --dalsi prikaz
				                end if;
				                next_state	<= WHILE_END3;                               --budeme cyklovat pokud cnt_data = 0
        

        

        when others => null;		                                                   --jestli nastane chyba nebo nejaky neznamy stav nic nedelame
		  
		  end case;

 end process fsm_main_process;





end behavioral;

