library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity vjtag is
	Port (LED : out std_logic_vector(7 downto 0));
end vjtag;

architecture rtl of vjtag is
	component vj is
		port (
			tdi                : out std_logic;                                       -- tdi
			tdo                : in  std_logic                    := 'X';             -- tdo
			ir_in              : out std_logic_vector(0 downto 0);                    -- ir_in
			ir_out             : in  std_logic_vector(0 downto 0) := (others => 'X'); -- ir_out
			virtual_state_cdr  : out std_logic;                                       -- virtual_state_cdr
			virtual_state_sdr  : out std_logic;                                       -- virtual_state_sdr
			virtual_state_e1dr : out std_logic;                                       -- virtual_state_e1dr
			virtual_state_pdr  : out std_logic;                                       -- virtual_state_pdr
			virtual_state_e2dr : out std_logic;                                       -- virtual_state_e2dr
			virtual_state_udr  : out std_logic;                                       -- virtual_state_udr
			virtual_state_cir  : out std_logic;                                       -- virtual_state_cir
			virtual_state_uir  : out std_logic;                                       -- virtual_state_uir
			tck                : out std_logic                                        -- clk
		);
	end component vj;

	signal tdi, tdo, cdr, sdr, e1dr, pdr, e2dr, udr, cir, uir, tck: std_logic;
	signal ir_in, ir_out : std_logic_vector(0 downto 0);
	signal tmp_reg : std_logic_vector(7 downto 0);
	signal load : std_logic;
	signal bypass_reg : std_logic;
begin

	vj0 : component vj
		port map (
			tdi                => tdi,                -- jtag.tdi
			tdo                => tdo,                --     .tdo
			ir_in              => ir_in,              --     .ir_in
			ir_out             => ir_out,             --     .ir_out
			virtual_state_cdr  => cdr,  --     .virtual_state_cdr
			virtual_state_sdr  => sdr,  --     .virtual_state_sdr
			virtual_state_e1dr => e1dr, --     .virtual_state_e1dr
			virtual_state_pdr  => pdr,  --     .virtual_state_pdr
			virtual_state_e2dr => e2dr, --     .virtual_state_e2dr
			virtual_state_udr  => udr,  --     .virtual_state_udr
			virtual_state_cir  => cir,  --     .virtual_state_cir
			virtual_state_uir  => uir,  --     .virtual_state_uir
			tck                => tck                 --  tck.clk
		);
		
		load <= ir_in(0);
		bypass_reg <= tdi;
		

   process (tck)
	begin
		if tck'event and tck = '1' then
			if load = '1' and sdr = '1' then
				tmp_reg <= tdi & tmp_reg(7 downto 1);
			end if;
		end if;
	end process;
	
	tdo <= tmp_reg(0) when load = '1' else bypass_reg;
	
	-- LED <= "11" & tmp_reg(5 downto 0);
	LED <= tmp_reg;
	
end rtl;

