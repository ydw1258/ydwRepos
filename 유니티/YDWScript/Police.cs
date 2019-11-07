using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Police : People
{
    public enum POLICESTATE
    {
        IDLE,
        WALK,
        RUN,
        ATTACK,
        SHOT,
        DIE
    }
    // Start is called before the first frame update
    public POLICESTATE policeState;
    public Animator anim;

    float patternChangeTimer = 3.0f;
    private float patternChangeInterval = 3.0f;
    float timer = 0;
    private int findRange = 10;
    //public
    float attackInterval = 1.0f;
    float curAttackCoolTime = 0.0f;

    float attackRange = 1.0f;
    public GameObject player;
    
    void Start()
    {
        player = GameObject.FindWithTag("Player");
        curAttackCoolTime = attackInterval;
    }
    // Start is called before the first frame update
    private void Update()
    {
        anim.SetInteger("CitizenState", (int)policeState);
        TimerCheck();
        if (policeState == POLICESTATE.WALK)
            Move();
        else if (policeState == POLICESTATE.RUN)
            ChasePlayer();
        else if (policeState == POLICESTATE.ATTACK && curAttackCoolTime > attackInterval)
        {
            curAttackCoolTime = 0;
            Attack();
        }
    }
    
    void TimerCheck()
    {
        timer += Time.deltaTime;

        patternChangeTimer += Time.deltaTime;

        if (DectectedPlayerAttack())
        {
            policeState = POLICESTATE.ATTACK;
        }

        if (policeState == POLICESTATE.ATTACK)
        {
            curAttackCoolTime += Time.deltaTime;
        }

        if (patternChangeTimer > 3.0f)
        {
            patternChangeTimer = 0.0f;

            switch (policeState)
            {
                case POLICESTATE.IDLE:
                    UpdatePeopleTargetDirection();
                    policeState = POLICESTATE.WALK;
                    break;
                case POLICESTATE.WALK:
                    policeState = POLICESTATE.IDLE;
                    break;
                case POLICESTATE.RUN:
                    break;
                case POLICESTATE.ATTACK:
                    curAttackCoolTime += Time.deltaTime;
                    break;
                case POLICESTATE.SHOT:
                    policeState = POLICESTATE.IDLE;
                    curAttackCoolTime += Time.deltaTime;
                    break;
                case POLICESTATE.DIE:
                    break;
            }
        }
    }
    bool DectectedPlayerAttack()
    {
        if (player.GetComponent<Player>().isAttack &&
            findRange > Vector3.Distance(transform.position, player.transform.position))
            return true;
        else
            return false;
    }
    void UpdatePeopleTargetDirection()
    {
        transform.Rotate(0, Random.Range(0, 360), 0);
    }
   
    void Attack()
    {
        //transform.LookAt(player.transform);
        player.GetComponent<Player>().Hurt(10);

        if (Vector3.Distance(transform.position, player.transform.position) > attackRange)
        {
            policeState = POLICESTATE.RUN;
        }
    }
}
