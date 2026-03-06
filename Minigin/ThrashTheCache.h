#pragma once  
#include <chrono>  
#include <vector>  

namespace ttc {  
   struct Transform  
   {  
       float matrix[16] = {  
           1,0,0,0,  
           0,1,0,0,  
           0,0,1,0,  
           0,0,0,1 };  
   };  

   class GameObject {  
   public:
       Transform local;  
       int id;
   };  

   class GameObjectAlt {
   public:
	   Transform* local;
       int id;
   };

   class ThrashTheCache  
   {  
   public:  
       std::vector<float> RunIntegerBenchmark() {
           std::vector<int> buffer(m_BufferSize);

           for (int step = 1; step <= 1024; step *= 2)
           {
               auto start = std::chrono::high_resolution_clock::now();
               for (size_t i = 0; i < m_BufferSize; i += step)
               {
                   buffer[i] *= 2;
               }

               auto end = std::chrono::high_resolution_clock::now();
               auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
               operationTime.push_back(static_cast<float>(elapsedTime));
           }
           return operationTime;
       }

       std::vector<float> RunGameObjectBenchmark() {
           std::vector<GameObject> buffer(m_BufferSize);

           for (int step = 1; step <= 1024; step *= 2)
           {
               auto start = std::chrono::high_resolution_clock::now();

               for (size_t i = 0; i < m_BufferSize; i += step)
               {
                   buffer[i].id *= 2;
               }

               auto end = std::chrono::high_resolution_clock::now();
               auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
               operationTime.push_back(static_cast<float>(elapsedTime));
           }
           return operationTime;
       }

       std::vector<float> RunGameObjectAltBenchmark() {
           std::vector<GameObjectAlt> buffer(m_BufferSize);

           for (int step = 1; step <= 1024; step *= 2)
           {
               auto start = std::chrono::high_resolution_clock::now();

               for (size_t i = 0; i < m_BufferSize; i += step)
               {
                   buffer[i].id *= 2;
               }

               auto end = std::chrono::high_resolution_clock::now();
               auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
               operationTime.push_back(static_cast<float>(elapsedTime));
           }
           return operationTime;
       }

   private:  
       std::vector<float> operationTime;

       const size_t m_BufferSize{ 1 << 26 };  
   };  
}
