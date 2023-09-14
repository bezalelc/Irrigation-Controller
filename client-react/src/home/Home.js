import { Tab, Tabs, TabList, TabPanel } from 'react-tabs';
import 'react-tabs/style/react-tabs.css'; // Import the default styles
import { useFirebase } from "../FirebaseContext";
import Area from "./Area";
import style from './Home.module.scss'

const Home = () => {
    const { data, dbPath } = useFirebase()
    const dbPathAreas = dbPath + '/areas'

    return (
        <div className={style.container}>
            <Tabs className={style.tabs} style={{ borderBottom: 'none' }} >
                <TabList className={style.tabList} >
                    {Object.entries(data.areas).map(([id, area]) => (
                        <Tab key={id} selectedClassName={style.selectedTab} className={`${style.tab} react-tabs__tab`}>{area.name}</Tab>
                    ))}
                </TabList>
                {Object.entries(data.areas).map(([id, area]) => (
                    <TabPanel key={id} className={style.tabPanel}>
                        <Area id={id} areaData={area} dbPathAreas={dbPathAreas} />
                    </TabPanel>
                ))}
            </Tabs>
        </div>
    );
}

export default Home;