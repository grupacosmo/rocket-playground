package pl.edu.pk.cosmo.rakieta.receiver;

import java.io.DataInputStream;
import java.io.IOException;

public interface ReceiverMicr{
    public void getPort(int BaudRate) throws IOException;
    public void readAsData(DataInputStream dis);
}
