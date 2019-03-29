#include "pch.h"
#include <iostream>
#include <tchar.h>

class Hardware : public CSerialEx, public DataPacketReceiver<PacketClassEnumerator> {
public:
	Hardware() : DataPacketReceiver<PacketClassEnumerator>(PACKET_HEADER) 
	{

	}

	unsigned char buffer[MAX_PAYLOAD_SIZE];
	unsigned int size;

	void init(string portName) {
		LONG    lLastError = ERROR_SUCCESS;
		std::wstring ws;
		ws.assign(portName.begin(), portName.end());
		lLastError = Open(&ws[0]);
		if (lLastError != ERROR_SUCCESS) {
			cout << "Unable to open COM-port: " << portName.c_str() << endl;
		}
		else {
			cout << "COM-port is open: " << portName.c_str() << endl;
		}
		CSerial::EBaudrate baud = CSerial::EBaud9600;
		CSerial::EStopBits stopb = CSerial::EStop2;
		CSerial::EParity parity = CSerial::EParNone;
		CSerial::EDataBits datab = CSerial::EData8;
		Setup(baud, datab, parity, stopb);
		SetupHandshaking(CSerial::EHandshakeOff);
		SetMask(CSerial::EEventBreak |
			CSerial::EEventCTS |
			CSerial::EEventDSR |
			CSerial::EEventError |
			CSerial::EEventRing |
			CSerial::EEventRLSD |
			CSerial::EEventRecv);
		StartListener();

	}
	void WritePacket(BaseDataPacket * packet) {
		size = packet->getSize();
		cout << "Write packet of size: " << size << endl;
		packet->toBytes(buffer, size);
		Write(buffer, size);
	}
	void WritePacketToFile(BaseDataPacket * packet, string filename) {
		size = packet->getSize();
		packet->toBytes(buffer, size);
		FILE* pFile;
		fopen_s(&pFile, filename.c_str(), "wb");
		fwrite(buffer, 1, size, pFile);
		fclose(pFile);
		cout << "Packet written to file '" << filename.c_str() << "'" << endl;
	}
	void	OnSerialEvent(EEvent eEvent, EError eError) {

		if (eEvent & CSerial::EEventRecv) {
			DWORD dwBytesRead = 0;
			char szBuffer[101];
			do {
				Read(szBuffer, sizeof(szBuffer) - 1, &dwBytesRead);

				if (dwBytesRead > 0) {
					szBuffer[dwBytesRead] = '\0';
				}
			} while (dwBytesRead == sizeof(szBuffer) - 1);

			for (unsigned int i = 0; i < dwBytesRead; i++) {
				readPacketByte(szBuffer[i]);
			}
		}
	}
	void OnReceivePacket(PacketClassEnumerator classType, unsigned char* buffer, unsigned short size) {

		if (classType == COMMAND) {

			cout << "Received COMMAND packet----------------------" << endl;

			DataPacket<CommandInStructure> packet;
			packet.fromBytes(buffer, size);

			if (packet.getPayload().command == GET_STATUS) {

				cout << "..Received GET_STATUS command" << endl;

				StatusDataPacket pack("EXTRBCRE", PacketClassEnumerator::STATUS);
				pack.setPayload(StatusOutStructure{ STATUS_IDLE, 1 });

				WritePacket(&pack);

			}
		}

	}
};


int main() {

	Hardware hw;

	/*
	unsigned char buffer[100];
	DataPacketReceiver hw;

	CompleteDataPacket<SimplePacket> pack("EXTRBCRE", PacketClassType::COMMAND, 2);
	pack.setPayload(SimplePacket{ 'G', 'L' });
	pack.calculateChecksum();
	pack.toBytes(buffer);

	//Random shit
	hw.readByte('d');
	hw.readByte('w');

	//Pack
	for (unsigned int i = 0; i < 18; i++){
		hw.readByte(buffer[i]);
	}

	//Random shit
	hw.readByte('d');
	hw.readByte('w');

	pack.setPayload(SimplePacket{ 'W', 'Q' });
	pack.calculateChecksum();
	pack.toBytes(buffer);

	//Pack
	for (unsigned int i = 0; i < 18; i++) {
		hw.readByte(buffer[i]);
	}

	//Random shit
	hw.readByte('d');
	hw.readByte('w');
	*/


	//CompleteDataPacket<SimplePacket> pack("EXTRBCRE", PacketClassType::SIMPLE_PACKET, 2);
	//pack.setPayload(SimplePacket{ 'G', 'L' });
	//hw.WritePacketToFile(&pack, "simplePacketTest.bin");



	hw.init("COM4");

	while (true) {}

	/**/
}