using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

public class PPTest : MonoBehaviour
{
    public Volume volume;
    public Blur blurVolume;

    // Start is called before the first frame update 
    void Start()
    {
        // find the Blur component in the volume that we've chosen 
        foreach (VolumeComponent vc in volume.profile.components)
        {
            Blur blur = vc as Blur;
            if (blur)
                blurVolume = blur;
        }
    }

    void Update()
    {
        if (blurVolume)
        {
            if (Input.GetKeyDown(KeyCode.LeftArrow))
                blurVolume.radius.value--;
            if (Input.GetKeyDown(KeyCode.RightArrow))
                blurVolume.radius.value++;
        }

    }
}
