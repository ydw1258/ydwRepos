using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class People : MonoBehaviour
{
    protected float rotateSpeed = 0.1f;
    protected float moveSpeed = 1.0f;
    protected float runSpeed = 4.0f;

    protected Vector3 movement;
    protected Vector3 direction;
    protected Vector3 targetVector = Vector3.zero;
    
    protected int hp = 100;
    protected int hDir = 0;
    protected int vDir = 0;
    
    protected bool isDie = false;
    public BoxCollider Collider;


    protected virtual void Move()
    {
        Vector3 Pos = transform.position;

        Pos.x += transform.forward.x * Time.deltaTime * moveSpeed;
        Pos.z += transform.forward.z * Time.deltaTime * moveSpeed;

        transform.position = Pos;
    }
    protected virtual void RunAway()
    {
        transform.LookAt(targetVector);
        transform.Rotate(0, 180, 0);

        Vector3 Pos = transform.position;

        Pos.x += transform.forward.x * Time.deltaTime * runSpeed;
        Pos.z += transform.forward.z * Time.deltaTime * runSpeed;

        transform.position = Pos;
    }
    protected virtual void ChasePlayer()
    {
        transform.LookAt(targetVector);

        Vector3 Pos = transform.position;

        Pos.x += transform.forward.x * Time.deltaTime * runSpeed;
        Pos.z += transform.forward.z * Time.deltaTime * runSpeed;

        transform.position = Pos;
    }
    public void Hurt(int damage)
    {
        hp -= damage;

        //사망시 true
        if (hp <= 0)
        {
            isDie = true;
        }
    }

    //Fixed Update에서 반드시 호출
    protected void UpdateTargetRotation()
    {
        targetVector = Vector3.zero;

        if (0 != hDir && 0 != vDir)
        {
            if (0 < hDir)
            {
                targetVector = new Vector3(0, (hDir * 90.0f) - (vDir * 45.0f), 0);
            }
            else if (0 > hDir)
            {
                targetVector = new Vector3(0, (hDir * 90.0f) + (vDir * 45.0f), 0);
            }
        }

        else if (0 != hDir)
        {
            targetVector = new Vector3(0, (hDir * 90.0f), 0);
        }

        else if (0 != vDir)
        {
            targetVector = new Vector3(0, (vDir * 90.0f - 90.0f), 0);
        }
    }
    protected void UpdateSlerpedRotation()
    {
        Quaternion rotation1 = Quaternion.Euler(transform.eulerAngles);
        Quaternion rotation2 = Quaternion.Euler(targetVector);

        if (0 != hDir || 0 != vDir)
        {
            transform.rotation = Quaternion.Slerp(rotation1, rotation2, rotateSpeed);
        }
    }
}
