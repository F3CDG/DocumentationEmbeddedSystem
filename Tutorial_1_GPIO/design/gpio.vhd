
--------------------------------------------------------------------------
-- Company: Gruppo IV - Sistemi Embedded 2016-17
-- Engineer: Colella Gianni, Guida Ciro, Lombardi Daniele
--
-- Create Date: 10.05.2017 12:24:39
-- Module Name: gpio - Dataflow
-- Target Devices: Zynq Z-7010
-- Tool Versions: Vivado 2016.4
--
--------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity gpio is
    Port ( pad_out : in STD_LOGIC;
           pad_rw_n : in STD_LOGIC;
           pad_en : in STD_LOGIC;
           pad_in : out STD_LOGIC;
           pad : inout STD_LOGIC);
end gpio;

architecture DataFlow of gpio is

begin

pad_in <= pad when pad_en='1' else '0';
pad <= 'Z' when pad_rw_n = '0' else pad_out and pad_en;

end DataFlow;
