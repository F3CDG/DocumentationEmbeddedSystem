
--------------------------------------------------------------------------
-- Company: Gruppo IV - Sistemi Embedded 2016-17
-- Engineer: Colella Gianni, Guida Ciro, Lombardi Daniele
--
-- Create Date: 10.05.2017 12:34:37
-- Module Name: gpio_array - Structural
-- Target Devices: Zynq Z-7010
-- Tool Versions: Vivado 2016.4
--
--------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity gpio_array is
    Generic ( gpio_size : natural := 8);
    Port ( pad_out : in STD_LOGIC_VECTOR (gpio_size-1 downto 0);
           pad_rw_n : in STD_LOGIC_VECTOR (gpio_size-1 downto 0);
           pad_en : in STD_LOGIC_VECTOR (gpio_size-1 downto 0);
           pad_in : out STD_LOGIC_VECTOR (gpio_size-1 downto 0);
           pad : inout STD_LOGIC_VECTOR (gpio_size-1 downto 0));
end gpio_array;

architecture Structural of gpio_array is

component gpio is
    Port ( pad_out : in STD_LOGIC;
           pad_rw_n : in STD_LOGIC;
           pad_en : in STD_LOGIC;
           pad_in : out STD_LOGIC;
           pad : inout STD_LOGIC);
end component;

begin

MULTI_GPIO : for i in 0 to gpio_size-1 generate
        SINGLE_GPIO : gpio port map (
                pad_rw_n=>pad_rw_n(i),
                pad_out => pad_out(i),
                pad_en => pad_en(i),
                pad => pad(i),
                pad_in => pad_in(i));
                
                end generate;
end Structural;
