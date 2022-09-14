from network_simulator import NetworkSimulator, Packet, EventEntity
from enum import Enum
from struct import pack, unpack

class GBNHost():

    # The __init__ method accepts:
    # - a reference to the simulator object
    # - the value for this entity (EntityType.A or EntityType.B)
    # - the interval for this entity's timer
    # - the size of the window used for the Go-Back-N algorithm
    def __init__(self, simulator, entity, timer_interval, window_size):
        
        # These are important state values that you will need to use in your code
        self.simulator = simulator
        self.entity = entity
        
        # Sender properties
        self.timer_interval = timer_interval        # The duration the timer lasts before triggering
        self.window_size = window_size              # The size of the seq/ack window
        self.window_base = 0                        # The last ACKed packet. This starts at 0 because no packets 
                                                    # have been ACKed
        self.next_seq_num = 0                       # The SEQ number that will be used next
        self.unACKed_buffer = {}
        self.app_layer_buffer = []
        self.expected_seq_num = 0
        self.last_ack = self.make_pkt(-1, None)

    
    def make_pkt(self, num, payload):
        if payload != None:
            msg = pack("!HiHI" + str(len(payload)) + "s", 128, num, 0, len(payload), payload.encode())
            checksum = self.compute_checksum(msg)
            msg = pack("!HiHI" + str(len(payload)) + "s", 128, num, checksum, len(payload), payload.encode())
          
        else:
            msg = pack("!HiHI", 0, num, 0, 0)
            checksum = self.compute_checksum(msg)
            msg = pack("!HiHI", 0, num, checksum, 0)
            
        return msg    

    ###########################################################################################################
    ## Core Interface functions that are called by Simulator

    # This function implements the SENDING functionality. It should implement retransmit-on-timeout. 
    # Refer to the GBN sender flowchart for details about how this function should be implemented
    def receive_from_application_layer(self, payload):
        if (self.next_seq_num < self.window_base + self.window_size):
            self.unACKed_buffer[self.next_seq_num] = self.make_pkt(self.next_seq_num, payload)
            self.simulator.pass_to_network_layer(self.entity, self.unACKed_buffer[self.next_seq_num], False)
            if (self.window_base == self.next_seq_num):
                self.simulator.start_timer(self.entity, self.timer_interval)
            self.next_seq_num += 1
        else:
            self.app_layer_buffer.append(payload)


    # This function implements the RECEIVING functionality. This function will be more complex that
    # receive_from_application_layer(), it includes functionality from both the GBN Sender and GBN receiver
    # FSM's (both of these have events that trigger on receive_from_network_layer). You will need to handle 
    # data differently depending on if it is a packet containing data, or if it is an ACK.
    # Refer to the GBN receiver flowchart for details about how to implement responding to data pkts, and
    # refer to the GBN sender flowchart for details about how to implement responidng to ACKs
    def receive_from_network_layer(self, bytes):
        unpack = None
        try:
            unpack = self.simulator.unpack_pkt(bytes)
        except Exception as e:
            None
            
        if (self.is_corrupt(bytes)):
            self.simulator.pass_to_network_layer(self.entity, self.last_ack, True)
            return
        if unpack.pkt_type == 0:
            if not self.is_corrupt(bytes):
                ack_num = unpack.pkt_number
                if (ack_num >= self.window_base):
                    self.window_base = ack_num + 1
                    self.simulator.stop_timer(self.entity)
                    if (self.window_base != self.next_seq_num):
                        self.simulator.start_timer(self.entity, self.timer_interval)
                    while (len(self.app_layer_buffer) > 0 and self.next_seq_num < self.window_base + self.window_size):
                        payload = self.app_layer_buffer.pop(0)
                        self.unACKed_buffer[self.next_seq_num] = self.make_pkt(self.next_seq_num, payload)
                        self.simulator.pass_to_network_layer(self.entity, self.unACKed_buffer[self.next_seq_num], False)
                        if (self.window_base == self.next_seq_num):
                            self.simulator.start_timer(self.entity, self.timer_interval)
                        self.next_seq_num += 1

        elif unpack.pkt_type == 128:       
                seq_num = unpack.pkt_number
                if (seq_num == self.expected_seq_num and not self.is_corrupt(bytes)):
                    data = unpack.payload
                    self.simulator.pass_to_application_layer(self.entity, data)
                    self.last_ack = self.make_pkt(self.expected_seq_num, None)
                    self.simulator.pass_to_network_layer(self.entity, self.last_ack, True)
                    self.expected_seq_num += 1

                elif (seq_num != self.expected_seq_num):
                    self.simulator.pass_to_network_layer(self.entity, self.last_ack, True)                


    # This function is called by the simulator when a timer interrupt is triggered due to an ACK not being 
    # received in the expected time frame. All unACKed data should be resent, and the timer restarted
    def timer_interrupt(self):
        self.simulator.start_timer(self.entity, self.timer_interval)
        for index in range (self.window_base, self.next_seq_num):
            self.simulator.pass_to_network_layer(self.entity, self.unACKed_buffer[index] , False)
            

    # This function should check to determine if a given packet is corrupt. The packet parameter accepted
    # by this function should contain a byte array
    def is_corrupt(self, packet):
        checksum = self.compute_checksum(packet)
        result = checksum == 0x0000
        return not result

    def compute_checksum(self, packet):
        s = 0x0000

        # If we have an odd number of bytes, pad the packet with 0x0000
        padded_pkt = None
        pkt_size = len(packet)
        if len(packet) % 2 == 1:
            padded_pkt = packet + bytes(1)
        else:
            padded_pkt = packet

        for i in range(0, len(padded_pkt), 2):
            w = padded_pkt[i] << 8 | padded_pkt[i+1]
            s = self.carry_around_add(s,w)

        checksum = ~s & 0xffff

        return checksum


    def carry_around_add(self, a, b):
        c = a + b
        return (c&0xffff) + (c >> 16)
    
    