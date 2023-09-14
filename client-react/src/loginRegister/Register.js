import { useEffect, useState } from "react";
import { useNavigate } from 'react-router-dom'
import { createUserWithEmailAndPassword } from "firebase/auth";
import { useFirebase } from "../FirebaseContext";
import EmailInput from "./EmailInput";
import PaswwordlInput from "./PasswordInput";
import SocialButton from "./SocialButton";
import style from './LoginRegister.module.scss'

const Register = () => {
    const [email, setEmail] = useState('')
    const [password, setPassword] = useState('')
    const { userId, firebaseAuth } = useFirebase()
    const navigate = useNavigate()

    useEffect(() => {
        if (userId) {
            navigate('/home')
        }
    }, [userId, navigate]);

    const registerWithEmailPasswordHandler = () => {
        createUserWithEmailAndPassword(firebaseAuth, email, password)
            .then(() => {
            })
            .catch((error) => {
                console.log(error.code)
                console.log(error.message)
                console.log(error)
            });
    }


    return (
        <div className={style.container} >
            <form className={style.form} onSubmit={(e) => e.preventDefault()}>
                <h1 className={style.h1}>Register</h1>
                <EmailInput email={email} setEmail={setEmail} />
                <PaswwordlInput password={password} setPassword={setPassword} />
                <div className={style.registerSpace}></div>
                <button type="submit" className={style.buttonSubmit} onClick={registerWithEmailPasswordHandler}>Register</button>
                <div className={style.dontHaveAccount}>Already have an account?{' '}
                    <button onClick={() => navigate('/')} className={style.register}>login</button>
                </div>
                <SocialButton />
            </form>
        </div>
    );
}

export default Register;