using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using Cinemachine;

public class AimOverrideControllerTPS : MonoBehaviour
{
    [Header("Camera Data")]
    [SerializeField][Tooltip("Camera used for aiming, set to control")]
    CinemachineVirtualCamera aimCam = null;
    [SerializeField][Tooltip("Camera's Standard sensitivity")]
    float camSensitivity;
    [SerializeField][Tooltip("Camera sensitivity while aiming")]
    float aimCamSensitivity;

    [Header("Layer Masking")]
    [SerializeField][Tooltip("layers to ignore")]
    LayerMask aimLayerMask = new LayerMask();

    [Header("Bullet Data")]
    [SerializeField][Tooltip("Provide a spawn position for a bullet")] //TODO
    Transform bulletSpawnPos;
    [SerializeField][Tooltip("Provides a bullet to be spawned")] //TODO
    GameObject bulletPrefab;

    PlayerControlsTPS controller;
    InputManagerTPS inputs;
    Animator animator;

    Vector2 currentanimationVec;
    Vector2 animationDirecton;

    [Space(20)]
    [Header("Other")]
    [Tooltip("How far can the bullet be shot for our hitscan gun.")]
    public float shotDistance = 200.0f;

    [Tooltip("Use to control the rate at which the layers in the animation change.")]
    public float animationlayerTransitionRate = 10.0f;

    [Tooltip("Use to control how far it projects on a miss. useful for aiming via animation riging")]
    public float missedRaycastDistance = 200.0f;

    public Transform aimMarkerTransform;

    void Awake()
    {
        controller = GetComponent<PlayerControlsTPS>();
        inputs = GetComponent<InputManagerTPS>();
        animator = GetComponent<Animator>();
    }

    void Update()
    {
        Vector2 movement = inputs.move.normalized;
        currentanimationVec = Vector2.SmoothDamp(currentanimationVec,
            movement, ref animationDirecton, 0.1f, 1.0f);

        animator.SetBool("isStillADS", movement == Vector2.zero ? true : false);
        animator.SetBool("isMotionADS", inputs.aim);
        animator.SetFloat("ForwardMotion", currentanimationVec.y);
        animator.SetFloat("RightMotion", currentanimationVec.x);

        Vector3 mouseWorldPos;
        Vector2 screenCenter = new Vector2(Screen.width / 2, Screen.height / 2);
        Ray ray = Camera.main.ScreenPointToRay(screenCenter);
        Transform hitRay = null;
        if (Physics.Raycast(ray, out RaycastHit raycastHit, shotDistance, aimLayerMask))
        {
            aimMarkerTransform.position = raycastHit.point;
            mouseWorldPos = raycastHit.point;
            hitRay = raycastHit.transform;
        }
        else
        {
            aimMarkerTransform.position = ray.GetPoint(missedRaycastDistance);
            mouseWorldPos = ray.GetPoint(missedRaycastDistance);
        }
            
        
        if (inputs.aim)
        {
            aimCam.gameObject.SetActive(true);
            controller.SetCamSensitivity(aimCamSensitivity);
            controller.SetRotationOnMove(false);
            animator.SetLayerWeight(1, Mathf.Lerp(animator.GetLayerWeight(1), 1.0f, Time.deltaTime * animationlayerTransitionRate));

            Vector3 aimTarget = mouseWorldPos;
            aimTarget.y = transform.position.y;
            Vector3 aimdirection = (aimTarget - transform.position).normalized;

            transform.forward = Vector3.Lerp(transform.forward, aimdirection, Time.deltaTime * 20);
        }
        else
        {
            aimCam.gameObject.SetActive(false);
            controller.SetCamSensitivity(aimCamSensitivity);
            controller.SetRotationOnMove(true);
            animator.SetLayerWeight(1, Mathf.Lerp(animator.GetLayerWeight(1), 0.0f, Time.deltaTime * animationlayerTransitionRate));
        }

    }
}
