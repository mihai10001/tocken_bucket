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
        //volatile double generatorPeriod @unit(ms) = default(100ms);
    gates:
        input inSource;
        output outSource;
        output outSourceFifo;
}

simple Fifo_file
{
    parameters:
        @display("i=block/queue;q=queue");
        //@signal[qlen](type="long");
        //@statistic[qlen](title="queue length"; record=vector,timeavg,max; interpolationmode=sample-hold);
    gates:
        //input rxScheduling;
        input inFifo;
        output outFifo;
}

simple Sink_file
{
    parameters:
        @display("i=block/sink");
        //int gateSize = default(100);
        //@signal[lifetime](type="simtime_t");
        //@statistic[lifetime](title="lifetime of arrived jobs"; unit=s; record=vector,mean,max; interpolationmode=none);
    gates:
        input inSink; //[gateSize];
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
        source.outSource --> {  delay = 100ms; } --> source.inSource;
        source.outSourceFifo --> {  delay = 100ms; } --> fifo.inFifo;
        fifo.outFifo --> {  delay = 100ms; } --> sink.inSink;
}
