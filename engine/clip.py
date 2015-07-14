
import frame
import node


class Clip(object):
    def __init__(self):
        self.frames = [] # ordered list of frames
        self.nodes  = [] # the network of processing nodes
        self.output_node = None # the target output node that must be linked

    def time_to_frame(self, time_norm):
        pass

