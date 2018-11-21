class Server {
  private:
    int listen_file_descriptor;

  public:
    Server();
    ~Server();
   
    
    void Run();

  private:
    void Connect();
    void Loop();
};
