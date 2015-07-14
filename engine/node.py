

class Data_Types:
    FRAME     = "frame"
    SELECTION = "selection"
    COLOR     = "color"
    NUMBER    = "number"



class Node(object):
    def __init__(self):
        pass

    def __call__(self):
        pass



class Node_Wrapper(object):
    def __init__(self):
        self.inputs = {} # dict of input variables (key = var_name, value = Node reference)

    def __call__(self, time_norm):
        """
            Main render function for a Node

            recursively __calls__ input nodes, until it reaches
            a terminal node. Once all input data is satisfied,
            frame/data rendering will be done.
        """
        for var in self.inputs:
            pass

        # return data

    def link_input(self, var, node):
        self.inputs[var] = node

    def unlink_input(self, var, node):
        del self.inputs[var]

    def set_node(self, node):
        """ sets the  """
        pass
