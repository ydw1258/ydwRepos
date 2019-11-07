using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : People
{
    public enum PLAYERSTATE_UNDER
    {
        IDLE,
        WALK,
        JUMP,
        DIE
    }
    public enum PLAYERSTATE_UPPER
    {
        IDLE,
        PUNCH,
        SHOT
    }
    private float playerMoveSpeed = 10.0f;
    // Start is called before the first frame update
    public PLAYERSTATE_UNDER playerStateUnder = PLAYERSTATE_UNDER.IDLE;
    public PLAYERSTATE_UPPER playerStateUpper = PLAYERSTATE_UPPER.IDLE;
    
    public bool isAttack = false; //프로퍼티
    protected float shotDelay = 0;

    public Animator under;
    public Animator upper;

    void Start()
    {
        moveSpeed = playerMoveSpeed;
    }

    private void Update()
    {
        if (playerStateUnder == PLAYERSTATE_UNDER.DIE)
            return;
        under.SetInteger("PlayerState", (int)playerStateUnder);
        upper.SetInteger("PlayerState", (int)playerStateUpper);
    }
    private void FixedUpdate()
    {
        UpdateInput();
        UpdateTargetRotation();
        UpdateSlerpedRotation();
    }

    void UpdateInput()
    {
        //방향키 조작
        MoveControl();
        ActiveControl();

        //아무 행동도 (하체) 안뛰고 있을 때
        if (isAnyActive())
        {
            playerStateUnder = PLAYERSTATE_UNDER.IDLE;
        }

        Move();
        //오브젝트 조작(근처의 탈 것 등등)
    }
    #region lowlevelCode
    protected override void Move()
    {
        Vector3 Pos = transform.position;

        Pos.x += hDir * Time.deltaTime * moveSpeed;
        Pos.z += vDir * Time.deltaTime * moveSpeed;

        transform.position = Pos;
    }
    void MoveControl()
    {
        if (Input.GetKey(KeyCode.UpArrow))
        {
            vDir = 1;
            playerStateUnder = PLAYERSTATE_UNDER.WALK;
        }
        if (Input.GetKey(KeyCode.DownArrow))
        {
            vDir = -1;
            playerStateUnder = PLAYERSTATE_UNDER.WALK;

        }
        if (Input.GetKey(KeyCode.RightArrow))
        {
            hDir = 1;
            playerStateUnder = PLAYERSTATE_UNDER.WALK;
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            hDir = -1;
            playerStateUnder = PLAYERSTATE_UNDER.WALK;
        }
        if (!Input.GetKey(KeyCode.UpArrow) && !Input.GetKey(KeyCode.DownArrow))
        {
            vDir = 0;
        }
        if (!Input.GetKey(KeyCode.RightArrow) && !Input.GetKey(KeyCode.LeftArrow))
        {
            hDir = 0;
        }
    }
    bool isAnyActive()
    {
        if (!Input.GetKey(KeyCode.UpArrow) && !Input.GetKey(KeyCode.DownArrow) && !Input.GetKey(KeyCode.RightArrow) && !Input.GetKey(KeyCode.LeftArrow))
            return true;
        else
            return false;
    }
    

    void ActiveControl()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            //총을 들고있는지, 무기의 타입에 따라 결정
            playerStateUpper = PLAYERSTATE_UPPER.PUNCH;
            isAttack = true;
        }
        if (Input.GetKeyUp(KeyCode.A))
        {
            playerStateUpper = PLAYERSTATE_UPPER.IDLE;
            isAttack = false;
        }
        if (Input.GetKeyDown(KeyCode.S))
        {
            print("Jump");
        }
    }
    #endregion
}

