set service_port 9998
set listen_address 127.0.0.1


proc websrv {channel_name client_address client_port} {
    global wait_connection

    while {1} {
	set message [gets $channel_name]
	if {[string length $message] > 0} {
	    set switches [split $message ","]
	    set cmd [lindex $switches 0]
	    if {$cmd == 0} {
		set numclocks [lindex $switches 1]
		set retval [device_run_test_idle -num_clocks $numclocks]
		puts $channel_name $retval
		flush $channel_name
	    } elseif {$cmd == 1} {
		set instance_index [lindex $switches 1]
		set ir_value [lindex $switches 2]
		set nocaptured [lindex $switches 3]
		if {$nocaptured == 0} {
		    set retval [device_virtual_ir_shift -instance_index $instance_index -ir_value $ir_value]
		} else {
		    set retval [device_virtual_ir_shift -instance_index $instance_index -ir_value $ir_value -no_captured_ir_value]
		}
		puts $channel_name $retval
		flush $channel_name
	    } elseif {$cmd == 2} {
		set dr_value [lindex $switches 1]
		set instace_index [lindex $switches 2]
		set dr_length [lindex $switches 3]
		set nocaptured [lindex $switches 4]
		if {$nocaptured == 0} {
		    set retval [device_virtual_dr_shift -dr_value $dr_value -instance_index $instace_index -length $dr_length -value_in_hex]
		    
		} else {
		    set retval [device_virtual_dr_shift -dr_value $dr_value -instance_index $instace_index -length $dr_length -no_captured_dr_value -value_in_hex]
		}
		puts $channel_name $retval
		flush $channel_name
	    } elseif {$cmd == 3} {
		set dr_value [lindex $switches 1]
		set instace_index [lindex $switches 2]
		set nocaptured [lindex $switches 3]
		if {$nocaptured == 0} {
		    set retval [device_virtual_dr_shift -dr_value $dr_value -instance_index $instace_index -length 1]
		    
		} else {
		    set retval [device_virtual_dr_shift -dr_value $dr_value -instance_index $instace_index -length 1 -no_captured_dr_value]
		}
		puts $channel_name $retval
		flush $channel_name
	    } elseif {$cmd == 4} {
		puts $channel_name "OK"
		flush $channel_name
		break
	    }
	}
	if {[eof $channel_name]} break
    }
    set wait_connection 1
}


proc main {argc argv} {
    global service_port
    global listen_address
    global wait_connection
    
    puts "Programming Hardwares:"
    foreach hardware_name [get_hardware_names] {
	puts $hardware_name
	if { [string match "MBFTDI-Blaster*" $hardware_name] } {
	    set blaster_name $hardware_name
	}
    }

    puts "\nSelect JTAG chain connected to $blaster_name $hardware_name.\n";

    puts "\nDevices on the JTAG chain:"
    foreach device_name [get_device_names -hardware_name $blaster_name] {
	puts $device_name
	if { [string match "*10M50*" $device_name] } {
	    set test_device $device_name
	}
    }

    open_device -hardware_name $blaster_name -device_name $test_device
    device_lock -timeout 10000

    set wait_connection 0
    set sock [socket -server websrv -myaddr $listen_address $service_port]
    puts "Marsohod listening on $listen_address:$service_port]"
    vwait wait_connection

    device_unlock
    close_device
    close $sock
}    
#    puts "Current LED Value (sample #[device_virtual_ir_shift -instance_index 0 -ir_value 1]): [device_virtual_dr_shift -instance_index 0 -length 6 -value_in_hex]"
#    device_unlock
#
#   puts "\nType in 2 digits in hexadecimal to update the LED:"
#    gets stdin update_value
#    device_lock -timeout 10000
#    device_virtual_ir_shift -instance_index 0 -ir_value 1 -no_captured_ir_value
#    device_virtual_dr_shift -instance_index 0 -length 6 -dr_value $update_value -value_in_hex -no_captured_dr_value
#    device_unlock
#    close_device


main $argc $argv
