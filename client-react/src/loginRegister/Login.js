import { useEffect, useState } from "react";
import { useNavigate } from 'react-router-dom'
import { useFirebase } from "../FirebaseContext";
import { signInWithEmailAndPassword } from "firebase/auth";
import EmailInput from "./EmailInput";
import PaswwordlInput from "./PasswordInput";
import SocialButton from "./SocialButton";
import SubmitButton from '../sharedComponents/SubmitButton'
import style from './LoginRegister.module.scss'

const Login = () => {
    const [email, setEmail] = useState('')
    const [password, setPassword] = useState('')
    const { userId, firebaseAuth } = useFirebase()
    const navigate = useNavigate()

    useEffect(() => {
        if (userId) {
            navigate('/home')
        } else {
            navigate('/');
        }
    }, [userId, navigate]);

    const loginWithEmailPasswordHandler = () => {
        signInWithEmailAndPassword(firebaseAuth, email, password)
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
                <h1 className={style.h1}>Login</h1>
                <EmailInput email={email} setEmail={setEmail} />
                <PaswwordlInput password={password} setPassword={setPassword} />
                <button onClick={() => navigate('/forgotPassword')} className={style.buttonForgotPassword}>Forgot password?</button>
                <SubmitButton text="Login" onClick={loginWithEmailPasswordHandler} className={style.buttonSubmit} />
                <div className={style.dontHaveAccount}>Don't have an account?{' '}
                    <button onClick={() => navigate('/register')} className={style.register}>register</button>
                </div>
                <SocialButton />
            </form>
        </div>
    );
}

export default Login;