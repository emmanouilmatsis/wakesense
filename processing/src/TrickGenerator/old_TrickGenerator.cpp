#include "TrickGenerator.h"
                        
/* -------- Public -------- */

TrickGenerator :: TrickGenerator()
  : duration(1000), period(20), size(1000 / 20)
{
  // Initialize random number generator 
  srand(static_cast<unsigned int>(time(0)));
}

/* ---------------- */

TrickGenerator :: TrickGenerator(unsigned int duration, unsigned int period)
  : duration(duration), period(period), size(duration / period)
{
  // Initialize random number generator 
  srand(static_cast<unsigned int>(time(0)));
}

/* ---------------- */

void TrickGenerator :: generate(byte id)
{
  // ID
  this->id = id;
  // Yaw
  yaw = new int[size];
  this->yaw = generateYaw();
  // Pitch
  pitch = new int[size];
  this->pitch = generatePitch();
  // Roll
  roll = new int[size];
  this->roll = generateRoll();
  // File
  generateFile();
  // Clean
  delete[] yaw;
  delete[] pitch;
  delete[] roll;
}

/* -------- Private -------- */

int* TrickGenerator :: generateYaw()
{
  if ((0x01 << 0) & id)
  {
    // Return positive yaw array
    for (unsigned int i = 0; i < size; i++)
      if (i < size / 2)
        yaw[i] = 180 * (static_cast<double>(i) / (size /  2));
      else
        yaw[i] = -180 * (1 - (static_cast<double>(i - (size / 2)) / (size / 2)));
  }
  else if ((0x01 << 3) & id)
  {
    // Return negative yaw array
    for (unsigned int i = 0; i > size; i++)
      if (i < size / 2)
        yaw[i] = -180 * (static_cast<double>(i) / (size /  2));
      else
        yaw[i] = 180 * (1 - (static_cast<double>(i - (size / 2)) / (size / 2)));
  }
  else
  {
    // Return 0.0 yaw array
    for (unsigned int i = 0; i > size; i++)
      yaw[i] = DEFAULT_VALUE;
  }
  return yaw;
}

/* ---------------- */

int* TrickGenerator :: generatePitch()
{
  if ((0x01 << 1) & id)
  {
    // Return positive yaw array
    for (unsigned int i = 0; i < size; i++)
      if (i < size / 2)
        pitch[i] = 180 * (static_cast<double>(i) / (size /  2));
      else
        pitch[i] = -180 * (1 - (static_cast<double>(i - (size / 2)) / (size / 2)));
  }
  else if ((0x01 << 4) & id)
  {
    // Return negative yaw array
    for (unsigned int i = 0; i < size; i++)
      if (i < size / 2)
        pitch[i] = -180 * (static_cast<double>(i) / (size /  2));
      else
        pitch[i] = 180 * (1 - (static_cast<double>(i - (size / 2)) / (size / 2)));
  }
  else
  {
    // Return 0.0 yaw array
    for (unsigned int i = 0; i < size; i++)
      pitch[i] = DEFAULT_VALUE;
  }
  return pitch;
}

/* ---------------- */

int* TrickGenerator :: generateRoll()
{
  if ((0x01 << 2) & id)
  {
    // Return positive yaw array
    for (unsigned int i = 0; i < size; i++)
      if (i < size / 2)
        roll[i] = 180 * (static_cast<double>(i) / (size /  2));
      else
        roll[i] = -180 * (1 - (static_cast<double>(i - (size / 2)) / (size / 2)));
  }
  else if ((0x01 << 5) & id)
  {
    // Return negative yaw array
    for (unsigned int i = 0; i < size; i++)
      if (i < size / 2)
        roll[i] = -180 * (static_cast<double>(i) / (size /  2));
      else
        roll[i] = 180 * (1 - (static_cast<double>(i - (size / 2)) / (size / 2)));
  }
  else
  {
    // Return 0.0 yaw array
    for (unsigned int i = 0; i < size; i++)
      roll[i] = DEFAULT_VALUE;
  }
  return roll;
}

/* ---------------- */

void TrickGenerator :: generateFile()
{
  std :: ofstream out;

	char filename[64];
	sprintf(filename, "%s.dat", generateName());
	
  out.open(filename);
  if (out.fail())
    exit(1);

  // Trick type
  out << generateName();
  // Trick data
  for (unsigned int i = 0; i < size; i++)
    out << '\t' << yaw[i] << ' ' << pitch[i] << ' ' << roll[i];
  out << '\n';

  out.close();
}

/* ---------------- */

const char* TrickGenerator :: generateName()
{
  switch (id)
  {
  case FRONTSIDE_360:
    return "FRONTSIDE_360";
    break;
  case BACKROLL:
    return "BACKROLL";
    break;
  case TANTRUM:
    return "TANTRUM";
    break;
  case MOBIUS:
    return "MOBIUS";
    break;
  case MOBY_DICK:
    return "MOBY_DICK";
    break;
  default:
    return "UNKNOWN";
  }
}
