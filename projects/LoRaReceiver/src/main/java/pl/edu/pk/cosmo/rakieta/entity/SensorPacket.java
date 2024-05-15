package pl.edu.pk.cosmo.rakieta.entity;

public class SensorPacket {
    public long time;
    public SensorPacketBME bme;
    public SensorPacketGPS gps;
    public SensorPacketMPU mpu;
    public int relayFlags;
}
