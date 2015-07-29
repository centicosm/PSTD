#pragma once

#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>

namespace PSTD {
	
#define MAX_TIMERS 16


	typedef std::chrono::milliseconds milliseconds;
	typedef std::chrono::microseconds microseconds;
	typedef std::chrono::microseconds nanoseconds;

	struct ProfilerSample {
		std::chrono::time_point<std::chrono::high_resolution_clock> _StartTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _StopTime;
		int64_t _AvgTime;
		unsigned int _SampleCnt;
	};


	class PProfiler {
		public:
		PProfiler(void) {};
		~PProfiler(void) {};

		void Start_Timer(unsigned int id) {
			_Samples[id]._StartTime = std::chrono::high_resolution_clock::now();
		};

		void Stop_Timer(unsigned int id) {
			_Samples[id]._StopTime = std::chrono::high_resolution_clock::now();
			int64_t microSec = std::chrono::duration_cast<nanoseconds>(_Samples[id]._StopTime - _Samples[id]._StartTime).count();
			_Samples[id]._AvgTime = ((_Samples[id]._AvgTime * _Samples[id]._SampleCnt) + microSec) / (++_Samples[id]._SampleCnt);
		};
			
		int64_t Get_Timer(unsigned int id) { return _Samples[id]._AvgTime; };
		
		void Clear_Timer(unsigned int id) { 
			_Samples[id]._AvgTime = 0;
			_Samples[id]._SampleCnt = 0;
		}

		void InitAndStart_Timer(unsigned int id) {
			Clear_Timer(id);
			Start_Timer(id);
		}

		ProfilerSample _Samples[MAX_TIMERS];
		

	};


};



#endif