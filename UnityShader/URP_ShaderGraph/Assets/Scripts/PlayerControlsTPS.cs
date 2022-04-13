using System.Collections;
using System.Collections.Generic;
using UnityEngine.InputSystem;
using UnityEngine;

[RequireComponent(typeof(CharacterController))]

public class PlayerControlsTPS : MonoBehaviour
{
    [Header("Player Movement")]
    [Tooltip("Movement speed of the player")]
    public float movementSpeed = 2.0f;
    [Tooltip("Sprinting Speed of the Player")]
    public float runningSpeed = 6.0f;
    [Tooltip("Player turn rate")]
    [Range(0.0f, 0.4f)]
    public float rotationSmoothRate = 0.1f;
    [Tooltip("Player rate of speed change")]
    public float speedChangeRate = 10.0f;

    [Space(10)]
    [Header("Player Grounded")]
    [Tooltip("Check if the Player is grounded")]
    public bool grounded = true;
    [Tooltip("Rough grounded offset, useful for complicated terrains")]
    [Range(-1, 1)]
    public float groundedOffSet = -0.15f;
    [Tooltip("Radius of the above check")]
    public float groundedRadius = 0.3f;
    [Tooltip("Layers approved to be the 'grounded'")]
    public LayerMask groundLayers;

    [Space(10)]
    [Header("Player CineMachine Camera Controls")]
    [Tooltip("Follow target set for the virtual cam that is active")]
    public GameObject CinemachineVirtualCamera;
    [Tooltip("In Degrees, how high up it moves")]
    public float topClamp = 70.0f;
    [Tooltip("In Degrees, how far down it moves")]
    public float bottomClamp = -30.0f;
    [Tooltip("Testing/Overloading the currently accepted degrees")]
    public float cameraClampOverride;
    [Tooltip("Locking the camera on each axis")]
    public bool lockCamPosition;
    [Tooltip("Camera sensitibity Speed adjustment")]
    public float cameraSensitivity;

    // Cinemachine Camera
    float cinemachineTargetYaw;
    float cinemachineTargetPitch;

    // Player Ground Settings
    float speed;
    float animationBlend;
    float rotation = 0;
    float rotationVelocity;
    float forwardVelocity;
    float maxForwardVelocity = 55;
    bool rotateOnMove = true;


    Animator animator;
    CharacterController characterController;
    InputManagerTPS input;
    GameObject mainCamera;

    bool hasAnimator;
    const float threshhold = 0.01f;

    private void Awake()
    {
        if (mainCamera == null)
            mainCamera = GameObject.FindGameObjectWithTag("MainCamera");
    }


    // Start is called before the first frame update
    void Start()
    {
        hasAnimator = TryGetComponent(out animator);
        characterController = GetComponent<CharacterController>();
        input = GetComponent<InputManagerTPS>();
    }

    // Update is called once per frame
    void Update()
    {
        GroundCheck();
        Moving();
    }

    void LateUpdate()
    {
        CameraRotation();
    }

    void GroundCheck()
    {
        Vector3 spherePosition = new Vector3(transform.position.x, transform.position.y - groundedOffSet, transform.position.z);
        grounded = Physics.CheckSphere(spherePosition, groundedRadius, groundLayers,
            QueryTriggerInteraction.Ignore);

        if (hasAnimator)
        {
            animator.SetBool("isGrounded", grounded);
        }
    }

    void CameraRotation()
    {
        if (input.look.sqrMagnitude >= threshhold && !lockCamPosition)
        {          
            cinemachineTargetYaw += input.look.x * Time.deltaTime * cameraSensitivity;
            cinemachineTargetPitch += input.look.y * Time.deltaTime * cameraSensitivity;
        }

        // Next we want to make sure the camera is clamped
        cinemachineTargetYaw = ClampAngle(cinemachineTargetYaw, float.MinValue, float.MaxValue);
        cinemachineTargetPitch = ClampAngle(cinemachineTargetPitch, bottomClamp, topClamp);

        CinemachineVirtualCamera.transform.rotation = Quaternion.Euler(cinemachineTargetPitch + cameraClampOverride,
            cinemachineTargetYaw, 0.0f);

    }

    void Moving()
    {
        // We need our speed to change depending on if the sprint key is pressed or not
        float targetSpeed = input.run ? runningSpeed : movementSpeed;

        if (input.move == Vector2.zero)   
            targetSpeed = 0.0f;

        // Next we need to grab the players current  sPeeds
        float currentHorizontalSpeed = new Vector3(characterController.velocity.x, 0.0f, 
            characterController.velocity.z).magnitude;
        float speedOffset = 1.0f;
        float inputMag = input.movement ? input.move.magnitude : 1.0f;

        // Now we adjust to the target speed
        if (currentHorizontalSpeed < targetSpeed - speedOffset ||
            currentHorizontalSpeed > targetSpeed + speedOffset)
        {
            speed = Mathf.Lerp(currentHorizontalSpeed, targetSpeed * inputMag, Time.deltaTime * speedChangeRate);
            speed = Mathf.Round(speed * 1000) / 1000f; // This will keep it at 3 decimal places.
        }
        else
        {
            speed = targetSpeed;
        }

        animationBlend = Mathf.Lerp(animationBlend, targetSpeed, Time.deltaTime * speedChangeRate);

        // We then need to normalise the input direction
        Vector3 inputdir = new Vector3(input.move.x, 0.0f, input.move.y).normalized;

        if (input.move != Vector2.zero)
        {
            rotation = Mathf.Atan2(inputdir.x, inputdir.z) * Mathf.Rad2Deg + mainCamera.transform.eulerAngles.y;
            float rotate = Mathf.SmoothDampAngle(transform.eulerAngles.y, rotation, ref rotationVelocity, rotationSmoothRate);
            if (rotateOnMove)      
                transform.rotation = Quaternion.Euler(0.0f, rotate, 0.0f);        
        }

        Vector3 targetDirection = Quaternion.Euler(0.0f, rotation, 0.0f) * Vector3.forward;

        characterController.Move(targetDirection.normalized * (speed * Time.deltaTime) +
            new Vector3(0.0f, forwardVelocity, 0.0f) * Time.deltaTime);

        if (hasAnimator)
        {
            Vector2 move = new Vector2(forwardVelocity, currentHorizontalSpeed).normalized;
            animator.SetFloat("Speed", animationBlend);
            animator.SetFloat("SpeedMultiplier", inputMag);
        }
    }

    static float ClampAngle(float a_angle, float a_min, float a_max)
    {
        if (a_angle < -360f)
            a_angle += 360;
        if (a_angle > 360f)
            a_angle -= 360;        

        return Mathf.Clamp(a_angle, a_min, a_max);
    }

    void OnDrawGizmosSelected()
    {
        Color transparantPurple = new Color(0.5f, 0.0f, 0.5f, 0.4f);
        Color transparantRed = new Color(1.0f, 0.0f, 0.0f, 0.5f);
       
        Gizmos.color = grounded ? transparantPurple : transparantRed;

        Gizmos.DrawSphere(new Vector3(transform.position.x, transform.position.y - 
            groundedOffSet, transform.position.z), groundedRadius);

    }

    public void SetCamSensitivity(float a_camSensitivity)
    {
        cameraSensitivity = a_camSensitivity;
    }

    public void SetRotationOnMove(bool a_rotate)
    {
        rotateOnMove = a_rotate;
    }





}
