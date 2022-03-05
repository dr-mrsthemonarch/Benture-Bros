// Control Models, functions.


float update_LQR_X(float * u)
{
  // LQR Controller with K gains for u[t] = K1*alpha+K2*alphadot+K3*omega 
  //float yr = -15.495174026892334 * (-M_PI + u[0]) - 3.4400308059648723 * u[1] + 0.01343605009462376 * u[2]; //weich
  float yr = -26.8591906126124*(-M_PI + u[0]) - 2.8517595773639597*u[1] + 0.018993162921974713*u[2];
  return yr;
}

float update_LQR_Y(float * u)
{
  // LQR Controller with K gains for u[t] = K1*alpha+K2*alphadot+K3*omega 
  //float yr = -15.495174026892334 * (-M_PI + u[0]) - 3.4400308059648723 * u[1] + 0.01343605009462376 * u[2]; //weich
  float yr = -26.8591906126124*(-M_PI + u[0]) - 2.8517595773639597*u[1] + 0.018993162921974713*u[2];
  return yr;
}

float update_LQR_Z(float * u)
{
  // LQR Controller with K gains for u[t] = K1*alpha+K2*alphadot+K3*omega 
  //float yr = -15.495174026892334 * (-M_PI + u[0]) - 3.4400308059648723 * u[1] + 0.01343605009462376 * u[2]; //weich
  float yr = -26.8591906126124*(-M_PI + u[0]) - 2.8517595773639597*u[1] + 0.018993162921974713*u[2];
  return yr;
}
//======================================================================
