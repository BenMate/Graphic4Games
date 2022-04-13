using System.Collections;
using System.Collections.Generic;
using UnityEngine.InputSystem;
using UnityEngine;

public class InputManagerTPS : MonoBehaviour
{
    // TPS inputs
    public Vector2 move;
    public Vector2 look;
    public bool jump;
    public bool aim;
    public bool shoot;
    public bool run;

    public bool movement;

    // Mouse lockers
    public bool cursorLocked = true;
    public bool cursorInputLock = true;

    #region Functions We Call On Button Press

    public void MoveInputs(Vector2 a_moveDirection)
    {
        move = a_moveDirection;
    }

    public void AimInput(bool a_aimState)
    {
        aim = a_aimState;
    }

    public void JumpInput(bool a_JumpState)
    {
        jump = a_JumpState;
    }

    public void LookInput(Vector2 a_lookDir)
    {
        look = a_lookDir;
    }

    public void ShootInput(bool a_shootState)
    {
        shoot = a_shootState;
    }

    public void RunInput(bool a_runState)
    {
        run = a_runState;
    }


    #endregion

    #region Functions Created For Connecting to Unity Input System

    public void OnMove(InputValue a_value)
    {
        MoveInputs(a_value.Get<Vector2>());
    }
    
    public void OnAim(InputValue a_value)
    {
        AimInput(a_value.isPressed);
    }

    public void OnJump(InputValue a_value)
    {
        JumpInput(a_value.isPressed);
    }
    public void OnShoot(InputValue a_value)
    {
        ShootInput(a_value.isPressed);
    }
    public void OnLook(InputValue a_value)
    {
        if (cursorLocked)
        {
            LookInput(a_value.Get<Vector2>());
        }
    }

    public void OnRun(InputValue a_value)
    {
        RunInput(a_value.isPressed);
    }





    #endregion

    void OnApplicationFocus(bool focus)
    {
        SetCurserState(cursorLocked);

    }

    void SetCurserState(bool a_state)
    {
        Cursor.lockState = a_state ? CursorLockMode.Locked : CursorLockMode.None;
    }

}
