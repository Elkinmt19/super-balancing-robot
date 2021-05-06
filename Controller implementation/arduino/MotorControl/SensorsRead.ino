// // Interrupciones asociadas el encoder de cuadratura del motor izquierdo 
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _LeftEncoderBSet = digitalReadFast(c_LeftEncoderPinB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef LeftEncoderIsReversed
    _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
  #else
    _LeftEncoderTicks += _LeftEncoderBSet ? -1 : +1;
  #endif
}
 
// // Interrupciones asociadas el encoder de cuadratura del motor derecho
//void HandleRightMotorInterruptA()
//{
//  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
//  _RightEncoderBSet = digitalReadFast(c_RightEncoderPinB);   // read the input pin
// 
//  // and adjust counter + if A leads B
//  #ifdef RightEncoderIsReversed
//    _RightEncoderTicks -= _RightEncoderBSet ? -1 : +1;
//  #else
//    _RightEncoderTicks += _RightEncoderBSet ? -1 : +1;
//  #endif
//}
