# foo_input_msu

MSU-1 PCM audio input decoder for [foobar2000](http://www.foobar2000.org/)

Implements basic playback and gapless looping

# Installation

Download foo_input_msu.dll and copy into your <foobar_install_dir>\components directory

# Building From Source

1. Download and install Visual Studio Express 2015
2. Download the latest [foobar2000 SDK](http://www.foobar2000.org/SDK) and extract it
3. Open the foobar2000 folder containing the various SDK projects
4. Check out this repository into its own folder alongside the existing SDK projects

e.g.

somedir\\  
|\`-foobar2000\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-ATLHelpers\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`--foo_input_msu\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;|\`-foobar2000\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;|\`-foo_input_msu.sln  
|&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\`-...  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-foo_input_validators\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-foo_sample\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-foobar2000_component_client\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-helpers\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-SDK\\  
|&nbsp;&nbsp;&nbsp;&nbsp;|\`-shared\\  
&nbsp;`-pfc\\  

5. If you want to be able to debug and execute directly from Visual Studio, download the latest release of [foobar2000](http://www.foobar2000.org/download) and do a portable installation into the foo_input_msu\\foobar2000 directory
6. Be sure to set the project build configuration to x86, then build the solution