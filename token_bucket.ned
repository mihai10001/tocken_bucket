//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

simple Source_file
{
    parameters:
        @display("i=block/source");
    gates:
        input inSource;
        output outSource;
        output outSourceFifo;
}

simple FIFORegulator_file
{
    parameters:
        @display("i=block/routing");
    gates:
        input inFifoRegPackets;
        input inFifoRegTokens;
        input inFifoRegFLC;
        output outFifoReg;
        output outFifoRegFLC;
}

simple Token_source_file
{
    parameters:
        @display("i=block/source");
    gates:
        input inSource;
        output outSource;
        output outSourceTokenFifo;
}

simple Token_fifo_file
{
    parameters:
        @display("i=block/queue;q=queue");
    gates:
        input inFifoToken;
        output outFifoToken;
}

simple Sink_file
{
    parameters:
        @display("i=block/sink");
    gates:
        input inSink;
}

simple FLC
{
    parameters:
        @display("i=block/cogwheel");
    gates:
        input inFLC;
        output outFLC;
}

network TokenBucket
{
    @display("bgb=1146.0487,664.90875");
    submodules:
        source: Source_file {
            @display("p=118,383;is=vl");
        }
        fifoReg: FIFORegulator_file {
            @display("p=447,383;is=vl");
        }
        tokenSource: Token_source_file {
            @display("p=447,78;is=vl");
        }
        tokenFifo: Token_fifo_file {
            @display("p=447,235;is=vl");
        }
        sink: Sink_file {
            @display("is=vl;p=762,383");
        }
        flc: FLC {
            @display("is=vl;p=446.61374,541.28253");
        }
    connections:
        // delay = the rate at which new IP Packets are created
        source.outSource --> {  delay = 50ms; } --> source.inSource;
        // delay = the rate at which new Tokens are created
        tokenSource.outSource --> {  delay = 100ms; } --> tokenSource.inSource;

        // delay = B-the burstiness at which IP Packets are delivered to the regulator
        source.outSourceFifo --> {  delay = 50ms; } --> fifoReg.inFifoRegPackets;
        // delay = B-the burstiness at which Tokens are delivered to the regulator ( both delays added )
        tokenSource.outSourceTokenFifo --> {  delay = 50ms; } --> tokenFifo.inFifoToken;
        tokenFifo.outFifoToken --> {  delay = 50ms; } --> fifoReg.inFifoRegTokens;

        // delay = R-continually sustainable data rate ( without accounting Tokens to regulator delay )
        fifoReg.outFifoReg --> {  delay = 50ms; } --> sink.inSink;

        // added FLC connections, the delay is not important
        fifoReg.outFifoRegFLC --> {  delay = 20ms; } --> flc.inFLC;
        flc.outFLC --> {  delay = 20ms; } --> fifoReg.inFifoRegFLC;
}
