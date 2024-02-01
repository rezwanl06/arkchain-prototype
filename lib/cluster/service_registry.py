class ServiceRegistry:
    def __init__(self, service_name, service_address):
        self.service_name = service_name
        self.service_address = service_address
        self.services = {}
    
    def register_user(self, username, port):
        if username not in self.services:
            self.services[username] = port
            print(f"Registered {username} {port}")
        
    def unregister_user(self, username):
        if username in self.services:
            del self.services[username]
            print(f"Unregistered {username}")
    
    def get_services(self):
        return [(key, val) for key, val in self.services.items()]

    def get_service_name(self):
        return self.service_name

    def get_service_address(self):
        return self.service_address
    
    def decode_message(self, message):
        message_words = message.split(" ")
        if(message_words[0] == "REGISTER"):
            if len(message_words) != 3:
                return "Format: REGISTER <username> <port>"
            username = message_words[1]
            port = int(message_words[2])
            self.register_user(username, port)
            return "Registration successful"
        elif(message_words[0] == "UNREGISTER"):
            if len(message_words)!= 2:
                return "Format: UNREGISTER <username>"
            username = message_words[1]
            self.unregister_user(username)
            return "Unregistration successful\n"
        else:
            return "Invalid request"