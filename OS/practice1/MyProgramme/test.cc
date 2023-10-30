#include <iostream>
#include <chrono>
int main()
{
  // std::cout << std::chrono::system_clock::now() << std::endl;

  auto start = std::chrono::steady_clock::now();
  std::cout << clock() << std::endl;
  clock_t end = clock() + 300000;
  std::cout << end << std::endl;
  std::cout << clock() << std::endl;

  while(true)
  {
    if(clock() > end)
    {
      std::cout << end << std::endl;
      std::cout << clock() << std::endl;
      break;
    }
  }
  printf ("It took me %ld clicks (%f seconds).\n", end,((float)end)/CLOCKS_PER_SEC);
  
  return 0;
}
