/** @file app/newprofiledialog/newprofiledialog.cpp
 * This file contains implementation of the NewProfileDialog class.
*/

#include <unistd.h>

#include <QFile>

#include "newprofiledialog/newprofiledialog.h"
#include "ui_newprofiledialog.h"

const QString noneProfile = "NONE";

NewProfileDialog::NewProfileDialog(const TunedManager *tunedManager, QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::NewProfileDialog)
    , tunedManager(tunedManager)
{
    ui -> setupUi(this);

    ui -> knownProfilesComboBox -> addItem(noneProfile);

    const QStringList availableProfiles = tunedManager -> GetAvailableProfiles();
    ui -> knownProfilesComboBox -> addItems(availableProfiles);

    connect(ui -> knownProfilesComboBox, SIGNAL(activated(int)), this, SLOT(onKnownProfileSelectedEvent(int)));

    ui -> knownProfilesComboBox->setCurrentIndex(0);
}

NewProfileDialog::~NewProfileDialog()
{
    delete ui;

}

#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusConnection>
#include <QtDBus>

// #include <polkit/polkit.h>
//

void NewProfileDialog::onKnownProfileSelectedEvent(int index)
{
    const QString text = ui -> knownProfilesComboBox -> itemText(index);
    const QString fileName = "/usr/lib/tuned/profiles/" + text + "/tuned.conf";

	qDBusRegisterMetaType<QMap<QString, QString>>();

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

	// const char* actionId = "org.freedesktop.policykit.exec";

	// PolkitAuthority *authority = polkit_authority_get_sync(NULL, NULL);

	// // PolkitSubject *subject = polkit_system_bus_name_get_sync(NULL, NULL);
	// // PolkitSubject *subject = polkit_unix_process_new(getuid());

	// const gint pid = getpid();
	// const gint uid = getuid();
	// PolkitSubject *subject = polkit_unix_process_new_for_owner(pid, 0, uid);

	// // PolkitAuthorizationResult *result = NULL;
	// GError *error = NULL;

	// PolkitAuthorizationResult *result = polkit_authority_check_authorization_sync(
	// authority
	// , subject
	// , actionId
	// , NULL
	// , POLKIT_CHECK_AUTHORIZATION_FLAGS_ALLOW_USER_INTERACTION
	// , NULL
	// , &error
	// );

	// if (error) {
	// // std::cerr << "Error checking authorization: " << error->message << std::endl;
	// g_error_free(error);
	// // return 1;
	// }

	// if (polkit_authorization_result_get_is_authorized(result)) {
	// // std::cout << "Action authorized." << std::endl;
	// } else {
	// // std::cout << "Action not authorized." << std::endl;
	// }

	// // g_object_unref(result);
	// // g_object_unref(subject);
	// // g_object_unref(authority);

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    QString actionId = "org.freedesktop.policykit.exec";  // Example action ID
	const quint32 pid = getpid();
	const quint32 uid = getuid();

    // Create the method call message
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.PolicyKit1"
        , "/org/freedesktop/PolicyKit1/Authority"
        , "org.freedesktop.PolicyKit1.Authority"
        , "CheckAuthorization"
    );

	const quint64 startTime = 0;

    // Create PolkitSubject as a QDBusArgument (struct with a string and a dictionary of variants)
    QDBusArgument subjectArg;
    subjectArg.beginStructure();
    subjectArg << QString("unix-process"); //QString::number(getuid());  // User ID as string
	subjectArg << QMap<QString, QVariant>({{"pid", pid}, {"uid", uid}, {"start-time", startTime}});
    subjectArg.endStructure();
    // QVariantMap subjectArg;
    // subjectArg["subject_kind"] = QString::number(getuid());
    // subjectArg["subject_details"] = QVariantMap();

//     // // Create a dictionary for the 'details' argument (empty here)
//     // QVariantMap details;
//     // // details["key"] = "value";  // Example of a key-value pair (can be empty if not needed)

//     // // Define flags (0 means no special flags)
    quint32 flags = 0x00000001;

    // Append parameters to the message using QDBusArgument for the subject (a structure)
    message << QVariant::fromValue(subjectArg)
            << actionId        // Action ID (string)
            << QVariant::fromValue(QMap<QString,QString>())          // Details (QVariantMap)
            << flags           // Flags (unsigned integer)
			<< ""
			;


    QDBusConnection theBus = QDBusConnection::systemBus();
    QDBusReply<QString> reply = theBus.call(message);

    if (reply.isValid()) {
        QString result = reply.value();
        if (result == "yes") {
            // std::cout << "User is authorized." << std::endl;
        } else if (result == "no") {
            // std::cout << "User is not authorized." << std::endl;
        } else {
            // std::cout << "Authorization result is unknown." << std::endl;
        }
    } else {
        // std::cerr << "D-Bus call failed: " << reply.error().message().toStdString() << std::endl;
        // return 1;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString profileContents = in.readAll();

    file.close();


    ui -> newProfileTextEdit -> setText(profileContents);

}