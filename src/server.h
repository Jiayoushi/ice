class Server {
  private:
    int server_file_descriptor;

  public:
    Server();
    ~Server();
   
    
    void Run();

  private:
    void Connect();
    void Loop();
};
