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

simple Token_source_file
{
    parameters:
        @display("i=block/source");
    gates:
        input inSource;
        output outSource;
        output outSourceTokenFifo;
}

simple Fifo_file
{
    parameters:
        @display("i=block/queue;q=queue");
    gates:
        input inFifo;
        output outFifo;
}

simple Sink_file
{
    parameters:
        @display("i=block/sink");
    gates:
        input inSink;
}

network TokenBucket
{
    submodules:
        source: Source_file {
            @display("p=75,210;is=vl");
        }
        fifo: Fifo_file {
            @display("p=332,210;is=vl");
        }
        sink: Sink_file {
            @display("p=577,210;is=vl");
        }
    connections:
        // delay = the rate at which new messages are created
        source.outSource --> {  delay = 10ms; } --> source.inSource;
        // delay = B-the burstiness at which messages are delivered
        source.outSourceFifo --> {  delay = 100ms; } --> fifo.inFifo;
        // delay = R=continually sustainable data rate 
        fifo.outFifo --> {  delay = 50ms; } --> sink.inSink;
}
